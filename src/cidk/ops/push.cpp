#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/push.hpp"

namespace cidk::ops {
  const PushType Push("push");

  PushType::PushType(string id): OpType(id) { }

  void PushType::init(Op &op, const Val &val) const {
    op.data = val;
  }

  void PushType::eval(const Op &op, Env &env) const {
    env.cx.stack.push_back(op.as<Val>());
  }

  void PushType::read(Cx &cx, const Pos &pos, Reader &in, Ops &out) const {
    Stack &s(cx.stack);

    for (;;) {
      auto p(pos);
      auto v(in.read_val());
      if (!v) { throw ReadE(p, "Missing ;"); }
      if (v->is_eol()) { break; }
      v->eval(in.env);
      out.emplace_back(p, *this, *pop(p, s, false));
    }
  }
}
