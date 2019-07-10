#ifndef CIDK_OP_SWEEP_HPP
#define CIDK_OP_SWEEP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct SweepType: OpType {
    SweepType(const string &id);
    void init(Op &op) const;
    virtual void eval(const Op &op, Env &env, Stack &stack) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      ReadState &state,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const SweepType Sweep;
}

#endif
