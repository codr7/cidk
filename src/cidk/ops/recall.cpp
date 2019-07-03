#include "cidk/cx.hpp"
#include "cidk/ops/recall.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const RecallType Recall("recall");

  RecallType::RecallType(string id): OpType(id) { }

  void RecallType::init(Op &op) const { }

  void RecallType::eval(const Op &op, Env &env, Stack &stack) const {
    env.cx.eval_state = EvalState::recall;
  }

  void RecallType::read(Cx &cx,
                        Pos &pos,
                        istream &in,
                        Env &env,
                        Stack &stack,
                        Ops &out) const {
    read_eop(pos, in, env, stack);
    out.emplace_back(pos, *this);
  }
}
