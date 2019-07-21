#include "cidk/cx.hpp"
#include "cidk/ops/recall.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const RecallType Recall("recall");

  RecallType::RecallType(const string &id): OpType(id) {}

  void RecallType::init(Cx &cx, Op &op) const {}

  void RecallType::eval(Op &op, Env &env, Stack &stack) const {
    env.cx.eval_state = EvalState::recall;
  }

  void RecallType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    read_eop(pos, in);
    out.emplace_back(cx, pos, *this);
  }
}
