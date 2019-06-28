#include "cidk/cx.hpp"
#include "cidk/ops/dup.hpp"

namespace cidk::ops {
  const DupType Dup("dup");

  DupType::DupType(string id): OpType(id) { }

  void DupType::init(Op &op) const { }

  void DupType::eval(Cx &cx, const Op &op) const {
    Stack &s(cx.stack);
    s.emplace_back(s.back());
  }

  void DupType::read(Cx &cx, const Pos &pos, Reader &in, Ops &out) const {
    out.emplace_back(pos, *this);
  }
}
