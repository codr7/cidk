#include "cidk/cx.hpp"
#include "cidk/ops/lit.hpp"

namespace cidk::ops {
  const LitType Lit("lit");

  LitType::LitType(string id): OpType(id) { }

  void LitType::eval(Cx &cx, const Op &op) const {
    cx.stack.push_back(op.as<Val>());
  }
}
