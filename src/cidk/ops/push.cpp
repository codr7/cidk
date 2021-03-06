#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"

namespace cidk::ops {
  const PushType Push("push");

  PushType::PushType(const string &id): OpType(id) {}

  void PushType::init(Cx &cx, Op &op, const Val &val) const { op.args[0] = val; }

  void PushType::compile(Cx &cx, Op &op, Env &env, Ops &out, Opts &opts) const {
    auto &v(op.args[0]);
    op.args[0].compile(op.pos, env, opts);

    if (v.type == &cx.expr_type && v.as_expr->flags(Expr::INLINE)) {
      auto &src(v.as_expr->ops);
      copy(src.begin(), src.end(), back_inserter(out));
    } else {
      out.push_back(op);
    }
  }

  bool PushType::find_op(Op &op, function<bool (Ops &, OpIter &)> pred) const {
    return op.args[0].find_op(pred);
  }

  void PushType::mark_refs(Op &op) const { op.args[0].mark_refs(); }

  void PushType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto v(read_val(cx, pos, in));
    if (!v || v->is_eop()) { throw ESys(p, "Missing value"); }
    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *v);
  }
}
