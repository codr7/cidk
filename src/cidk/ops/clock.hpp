#ifndef CIDK_OP_CLOCK_HPP
#define CIDK_OP_CLOCK_HPP

#include "cidk/op.hpp"

namespace cidk::ops {
  struct ClockType: OpType {
    ClockType(const string &id);

    void init(Cx &cx, Op &op, const Val &nreps, const Val &body) const;

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

  extern const ClockType Clock;
}

#endif
