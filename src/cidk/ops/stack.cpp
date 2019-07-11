#include "cidk/cx.hpp"
#include "cidk/ops/stack.hpp"
#include "cidk/list.hpp"
#include "cidk/read.hpp"
#include "cidk/types/list.hpp"

namespace cidk::ops {
  const StackType Stack("stack");

  StackType::StackType(const string &id): OpType(id) {}

  void StackType::init(Op &op) const {}

  void StackType::eval(const Op &op, cidk::Env &env, cidk::Stack &stack) const {
    Cx &cx(env.cx);
    List *out(cx.list_type.pool.get(cx));
    move(stack.begin(), stack.end(), back_inserter(out->items));
    stack.clear();
    stack.emplace_back(op.pos, env.cx.list_type, out);
  }

  void StackType::read(Cx &cx,
                       Pos &pos,
                       istream &in,
                       ReadState &state,
                       cidk::Env &env,
                       cidk::Stack &stack,
                       Ops &out) const {
    Pos p(pos);
    read_eop(pos, in, env, stack);
    out.emplace_back(p, *this);
    cout << "StackType::read" << endl;
  }
}
