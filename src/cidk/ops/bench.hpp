#ifndef CIDK_OP_BENCH_HPP
#define CIDK_OP_BENCH_HPP

#include "cidk/op.hpp"

namespace cidk::ops {
  struct BenchType: OpType {
    BenchType(string id);

    void init(Op &op, const Val &nreps, const Val &body) const;
    virtual void eval(const Op &op, Env &env, Stack &stack) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const BenchType Bench;
}

#endif
