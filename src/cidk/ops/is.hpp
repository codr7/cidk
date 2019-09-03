#ifndef CIDK_OP_IS_HPP
#define CIDK_OP_IS_HPP

#include "cidk/op.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct IsType: OpType {
    IsType(const string &id);
    void init(Cx &cx, Op &op, const Val &x, const Val &y) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual bool eval(Cx &cx, Op &op, Env &env, Reg *regs) const override {
      auto &p(op.pos);
      auto &args(op.args);

      for (int i(0); i < 2; i++) {
        if (!args[i].eval(p, env, regs)) { return false; }
      }
      
      auto &y(cx.pop(p)), &x(cx.peek(p));    
      x.reset(cx.bool_type, x.is(y));
      return true;
    }

    virtual bool find_op(Op &op,
                         function<bool (Ops &, OpIter &)> pred) const override;

    virtual void mark_refs(Op &op) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const IsType Is;
}

#endif
