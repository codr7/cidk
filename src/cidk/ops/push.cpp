#include "cidk/cx.hpp"
#include "cidk/ops/push.hpp"

namespace cidk::ops {
  const PushType Push("push");

  PushType::PushType(string id): OpType(id) { }

  void PushType::eval(Cx &cx, const Op &op) const {
    cx.stack.push_back(op.as<Val>());
  }
}
