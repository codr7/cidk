#ifndef CIDK_OP_DO_ENV_HPP
#define CIDK_OP_DO_ENV_HPP

#include "cidk/op.hpp"

namespace cidk::ops {
  struct DoEnvType: OpType {
    DoEnvType(const string &id);
    void init(Cx &cx, Op &op, const Val &in, const Val &body) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Stack &stack,
                         Ops &out,
                         Opts &opts) const override;

    virtual void eval(Cx &cx,
                      Op &op,
                      Env &env,
                      Reg *regs,
                      Stack &stack) const override;
    
    virtual void mark_refs(Op &op) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const DoEnvType DoEnv;
}

#endif
