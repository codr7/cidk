#ifndef CIDK_OP_LET_HPP
#define CIDK_OP_LET_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"
#include "cidk/types/reg.hpp"

namespace cidk::ops {
  inline void bind_id(Cx &cx, const Pos &pos,
                      const Val &id, const Val &val,
                      Reg *regs) {
    if (id.type == &cx.pair_type) {
      if (val.type != &cx.pair_type) { throw ESys(pos, "Expected pair: ", val);}
      auto &ip(*id.as_pair), vp(*val.as_pair);
      bind_id(cx, pos, ip.first, vp.first, regs);
      bind_id(cx, pos, ip.second, vp.second, regs);
    } else {
      if (id.type != &cx.reg_type) { throw ESys(pos, "Expected reg: ", id); }
      set_reg(regs, id.as_reg, id.id, val);
    }
  }
  
  struct LetType: OpType {    
    LetType(const string &id);
    void init(Cx &cx, Op &op, const Val &id, const Val &val) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;
    
    virtual bool eval(Cx &cx, Op &op, Env &env, Reg *regs) const override {
      auto &p(op.pos);
      auto &args(op.args);
      if (!args[1].eval(p, env, regs)) { return false; }
      bind_id(cx, p, args[0], cx.pop(p), regs);
      return true;
    }

    virtual bool find_op(Op &op,
                         function<bool (Ops &, OpIter &)> pred) const override;

    virtual void mark_refs(Op &op) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const LetType Let;
}

#endif
