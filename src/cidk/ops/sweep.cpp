#include "cidk/cx.hpp"
#include "cidk/ops/sweep.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const SweepType Sweep("sweep");

  SweepType::SweepType(const string &id): OpType(id) {}

  void SweepType::init(Cx &cx, Op &op) const {}

  bool SweepType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    cx.sweep_refs(op.pos);
    return true;
  }

  void SweepType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    read_eop(pos, in);
    out.emplace_back(cx, pos, *this);
  }
}
