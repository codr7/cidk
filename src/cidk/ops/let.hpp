#ifndef CIDK_OP_LET_HPP
#define CIDK_OP_LET_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct LetType: OpType {
    LetType(const string &id);
    void init(Cx &cx, Op &op, const Val &id, const Val &val) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual void eval(Cx &cx, Op &op, Env &env, Reg *regs) const override {
      auto &p(op.pos);
      auto &args(op.args);
      args[1].eval(p, env, regs);
      set_reg(regs, args[2].as_reg, args[0].as_sym, cx.pop(p));
    }
    
    virtual void mark_refs(Op &op) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const LetType Let;
}

#endif
