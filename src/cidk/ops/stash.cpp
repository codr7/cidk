#include "cidk/cx.hpp"
#include "cidk/ops/stash.hpp"
#include "cidk/list.hpp"
#include "cidk/read.hpp"
#include "cidk/types/list.hpp"

namespace cidk::ops {
  const StashType Stash("stash");

  StashType::StashType(const string &id): OpType(id) {}

  void StashType::init(Cx &cx, Op &op) const {}

  void StashType::eval(Op &op, cidk::Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    List *out(cx.list_type.pool.get(cx));
    move(stack.begin(), stack.end(), back_inserter(out->items));
    stack.clear();
    stack.emplace_back(op.pos, env.cx.list_type, out);
  }

  void StashType::read(Cx &cx,
                       Pos &pos,
                       istream &in,
                       ReadState &state,
                       cidk::Env &env,
                       Stack &stack,
                       Ops &out) const {
    Pos p(pos);
    read_eop(pos, in, env, stack);
    out.emplace_back(cx, p, *this);
  }
}
