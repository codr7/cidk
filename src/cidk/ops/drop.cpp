#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/drop.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const DropType Drop("drop");

  DropType::DropType(const string &id): OpType(id) { }

  void DropType::init(Op &op) const { }

  void DropType::eval(const Op &op, Env &env, Stack &stack) const {
    if (stack.empty()) { throw ESys(op.pos, "Nothing to drop"); }
    stack.pop_back();
  }

  void DropType::read(Cx &cx,
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
