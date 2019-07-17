#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const PushType Push("push");

  PushType::PushType(const string &id): OpType(id) {}

  void PushType::init(Op &op, const Val &val) const { op.data = val; }

  void PushType::eval(const Op &op, Env &env, Stack &stack) const {
    const Pos &p(op.pos);
    
    Val v;
    op.as<Val>().clone(p, v);
    v.eval(p, env, stack);
  }

  void PushType::get_ids(const Op &op, IdSet &out) const {
    op.as<Val>().get_ids(out);
  }

  void PushType::mark_refs(Op &op) const { op.as<Val>().mark_refs(); }

  void PushType::read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      ReadState &state,
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    Pos p(pos);
    int n(0);

    for (;; n++) {
      auto v(read_val(pos, in, state, env, stack));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      out.emplace_back(p, *this, *v);
    }

    if (!n) { throw ESys(p, "Missing push value"); }
  }
}
