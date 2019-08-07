#ifndef CIDK_OP_SWEEP_HPP
#define CIDK_OP_SWEEP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct SweepType: OpType {
    SweepType(const string &id);
    void init(Cx &cx, Op &op) const;

    virtual void eval(Cx &cx,
                      Op &op,
                      Env &env,
                      Reg *regs,
                      Stack &stack) const override;
    
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const SweepType Sweep;
}

#endif
