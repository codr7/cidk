#include "cidk/cx.hpp"
#include "cidk/ops/stash.hpp"
#include "cidk/list.hpp"
#include "cidk/read.hpp"
#include "cidk/types/list.hpp"

namespace cidk::ops {
  const StashType Stash("stash");

  StashType::StashType(const string &id): OpType(id) {}

  void StashType::init(Cx &cx, Op &op) const {}

  void StashType::eval(Cx &cx,
                       Op &op,
                       cidk::Env &env,
                       Reg *regs) const {
    List *l(cx.list_type.pool.get(cx));
    copy(cx.stack.begin(), cx.stackp, back_inserter(l->items));
    cx.stackp = &cx.stack[0];
    cx.push(op.pos, cx.list_type, l);
  }

  void StashType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    read_eop(pos, in);
    out.emplace_back(cx, p, *this);
  }
}
