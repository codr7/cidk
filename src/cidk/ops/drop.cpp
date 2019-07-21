#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/drop.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const DropType Drop("drop");

  DropType::DropType(const string &id): OpType(id) {}

  void DropType::init(Cx &cx, Op &op) const {}

  void DropType::eval(Cx &cx, Op &op, Env &env, Stack &stack) const {
    if (stack.empty()) { throw ESys(op.pos, "Nothing to drop"); }
    stack.pop_back();
  }

  void DropType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    read_eop(pos, in);
    out.emplace_back(cx, pos, *this);
  }
}
