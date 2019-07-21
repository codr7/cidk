#ifndef CIDK_OP_ENV_HPP
#define CIDK_OP_ENV_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct EnvType: OpType {
    EnvType(const string &id);
    void init(Cx &cx, Op &op) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         cidk::Env &env,
                         Stack &stack,
                         Ops &out,
                         Opts *opts) const override;

    virtual void eval(Op &op, cidk::Env &env, Stack &stack) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const EnvType Env;
}

#endif
