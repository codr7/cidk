#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const PushType Push("push");

  PushType::PushType(const string &id): OpType(id) {}

  void PushType::init(Cx &cx, Op &op, const Val &val) const { op.data = val; }

  void PushType::compile(Cx &cx,
                        Op &op,
                        Env &env,
                        Stack &stack,
                        Ops &out,
                        Opts *opts) const {
    op.as<Val>().compile(cx, op.pos, env, stack, opts);
    out.push_back(op);
  }

  void PushType::eval(Op &op, Env &env, Stack &stack) const {
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
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    Pos p(pos);
    int n(0);

    for (;; n++) {
      auto v(read_val(pos, in, env, stack));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      out.emplace_back(cx, p, *this, *v);
    }

    if (!n) { throw ESys(p, "Missing push value"); }
  }
}
