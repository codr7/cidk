#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const PushType Push("push");

  PushType::PushType(string id): OpType(id) { }

  void PushType::init(Op &op, const Val &val) const {
    op.data = val;
  }

  void PushType::eval(const Op &op, Env &env, Stack &stack) const {
    op.as<Val>().eval(op.pos, env, stack);
  }

  void PushType::read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    auto p(pos);
    int n(0);

    for (;; n++) {
      auto v(read_val(pos, in, env, stack));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      out.emplace_back(p, *this, *v);
    }

    if (!n) { throw ESys(p, "Missing push value"); }
  }
}
