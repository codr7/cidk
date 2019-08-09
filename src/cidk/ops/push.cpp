#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const PushType Push("push");

  PushType::PushType(const string &id): OpType(id) {}

  void PushType::init(Cx &cx, Op &op, const Val &val) const { op.data = val; }

  void PushType::compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const {
    in->as<Val>().compile(in->pos, env, opts);
    out.push_back(*in);
  }

  void PushType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    op.as<Val>().eval(op.pos, env, regs);
  }

  void PushType::mark_refs(Op &op) const { op.as<Val>().mark_refs(); }

  void PushType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    int n(0);

    for (;; n++) {
      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      out.emplace_back(cx, p, *this, *v);
    }

    if (!n) { throw ESys(p, "Missing push value"); }
  }
}
