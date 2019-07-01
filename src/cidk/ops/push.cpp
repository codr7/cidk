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
    op.as<Val>().eval(env);
  }

  void PushType::read(Cx &cx, const Pos &pos, Reader &in, Env &env, Ops &out) const {
    auto p(pos);
    int n(0);

    for (;; n++) {
      auto v(in.read_val(env));
      if (!v) { throw ERead(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      out.emplace_back(p, *this, *v);
    }

    if (!n) { out.emplace_back(p, *this, cx._); }
  }
}
