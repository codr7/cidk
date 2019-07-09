#include "cidk/cx.hpp"
#include "cidk/ops/dup.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const DupType Dup("dup");

  DupType::DupType(const string &id): OpType(id) {}

  void DupType::init(Op &op) const {}

  void DupType::eval(const Op &op, Env &env, Stack &stack) const {
    Val v(stack.back());
    stack.emplace_back(v);
  }

  void DupType::read(Cx &cx,
                     Pos &pos,
                     istream &in,
                     ReadState &state,
                     Env &env,
                     Stack &stack,
                     Ops &out) const {
    read_eop(pos, in, env, stack);
    out.emplace_back(pos, *this);
  }
}
