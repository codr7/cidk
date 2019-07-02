#include "cidk/cx.hpp"
#include "cidk/ops/recall.hpp"

namespace cidk::ops {
  const RecallType Recall("recall");

  RecallType::RecallType(string id): OpType(id) { }

  void RecallType::init(Op &op) const { }

  void RecallType::eval(const Op &op, Env &env) const {
    env.cx.eval_state = EvalState::recall;
  }

  void RecallType::read(Cx &cx,
                        const Pos &pos,
                        Reader &in,
                        Env &env,
                        Ops &out) const {
    in.read_eop(env);
    out.emplace_back(pos, *this);
  }
}
