#include "cidk/cx.hpp"
#include "cidk/ops/mark.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const MarkType Mark("mark");

  MarkType::MarkType(const string &id): OpType(id) {}

  void MarkType::init(Op &op) const {}

  void MarkType::eval(const Op &op, Env &env, Stack &stack) const {
    env.cx.mark_refs();
  }

  void MarkType::read(Cx &cx,
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