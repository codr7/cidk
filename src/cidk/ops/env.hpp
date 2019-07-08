#ifndef CIDK_OP_ENV_HPP
#define CIDK_OP_ENV_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct EnvType: OpType {
    EnvType(const string &id);
    void init(Op &op) const;
    virtual void eval(const Op &op, cidk::Env &env, Stack &stack) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      cidk::Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const EnvType Env;
}

#endif
