#ifndef CIDK_OP_SWAP_HPP
#define CIDK_OP_SWAP_HPP

#include "cidk/op.hpp"
#include "cidk/types/reg.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  inline Val &get_ref(Cx &cx,
                      const Pos &pos,
                      Val &place,
                      Env &env,
                      Reg *regs) {
    if (place.type == &cx.int_type) {
      return cx.stack[cx.stackp - cx.stack.begin() - place.as_int - 1];
    }

    return (place.type == &cx.reg_type)
      ? regs[place.as_reg]
      : env.get(pos, place.as_sym);
  }
  
  struct SwapType: OpType {
    SwapType(const string &id);
    void init(Cx &cx, Op &op, const Val &x, const Val &y) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual void eval(Cx &cx, Op &op, Env &env, Reg *regs) const override {
      auto &p(op.pos);
      auto &args(op.args);
      
      swap(get_ref(cx, p, args[0], env, regs),
           get_ref(cx, p, args[1], env, regs));
    }

    virtual void mark_refs(Op &op) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const SwapType Swap;
}

#endif
