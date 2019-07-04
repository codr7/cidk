#ifndef CIDK_OP_DO_ENV_HPP
#define CIDK_OP_DO_ENV_HPP

#include "cidk/op.hpp"

namespace cidk::ops {
  struct DoEnvType: OpType {
    DoEnvType(string id);
    void init(Op &op, const Val &body) const;
    virtual void eval(const Op &op, Env &env, Stack &stack) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const DoEnvType DoEnv;
}

#endif
