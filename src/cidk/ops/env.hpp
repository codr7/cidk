#ifndef CIDK_OP_ENV_HPP
#define CIDK_OP_ENV_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct EnvType: OpType {
    EnvType(const string &id);
    void init(Cx &cx, Op &op) const;
    virtual void eval(Op &op, cidk::Env &env, Stack &stack) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      ReadState &state,
                      cidk::Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const EnvType Env;
}

#endif
