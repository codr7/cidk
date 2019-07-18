#include "cidk/cx.hpp"
#include "cidk/ops/sweep.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const SweepType Sweep("sweep");

  SweepType::SweepType(const string &id): OpType(id) {}

  void SweepType::init(Cx &cx, Op &op) const {}

  void SweepType::eval(Op &op, Env &env, Stack &stack) const {
    env.cx.sweep_refs(op.pos);
  }

  void SweepType::read(Cx &cx,
                     Pos &pos,
                     istream &in,
                     ReadState &state,
                     Env &env,
                     Stack &stack,
                     Ops &out) const {
    read_eop(pos, in, env, stack);
    out.emplace_back(cx, pos, *this);
  }
}
