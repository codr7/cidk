#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/expr.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/ops/defer.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"

namespace cidk::ops {
  const DeferType Defer("defer");

  DeferType::DeferType(const string &id): OpType(id) {}

  void DeferType::init(Cx &cx, Op &op, const Val &body) const { op.args[0] = body; }

  void DeferType::compile(Cx &cx,
                          OpIter &in,
                          const OpIter &end,
                          Env &env,
                          Ops &out,
                          Opts &opts) const {
    auto &p(in->pos);
    auto &body(in->args[0]);
    body.compile(p, env, opts);
    if (body.type != &cx.expr_type) { throw ESys(p, "Invalid body: ", body); }
    out.push_back(*in);
  }

  void DeferType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    *cx.deferp = make_pair(op.pos, op.args[0]);
    cx.deferp++;
  }

  bool DeferType::find_op(Op &op, function<bool (Ops &, OpIter &)> pred) const {
    return op.args[0].find_op(pred);
  }

  void DeferType::mark_refs(Op &op) const { op.args[0].mark_refs(); }

  void DeferType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing body"); }
    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *body);
  }
}
