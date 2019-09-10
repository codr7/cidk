#ifndef CIDK_OP_LET_HPP
#define CIDK_OP_LET_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"
#include "cidk/types/reg.hpp"

namespace cidk::ops {
  struct LetType: OpType {    
    LetType(const string &id);
    void init(Cx &cx, Op &op, const Val &id, const Val &val) const;

    virtual void compile(Cx &cx,
                         Op &op,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;
    
    virtual bool eval(Cx &cx, Op &op, Env &env, Reg *regs) const override {
      auto &p(op.pos);
      auto &args(op.args);
      auto &id(args[0]), &val(args[1]);
      if (id.type != &cx.reg_type) { throw ESys(p, "Expected reg: ", id); }
      if (!val.eval(p, env, regs)) { return false; }
      set_reg(regs, id.as_reg, id.id, cx.pop(p));
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
