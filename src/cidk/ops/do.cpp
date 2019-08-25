#include "cidk/cx.hpp"
#include "cidk/defer.hpp"
#include "cidk/e.hpp"
#include "cidk/expr.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/ops/do.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"

namespace cidk::ops {
  const DoType Do("do");

  DoType::DoType(const string &id): OpType(id) {}

  void DoType::init(Cx &cx, Op &op, const Val &body) const { op.args[0] = body; }

  void DoType::compile(Cx &cx,
                       OpIter &in,
                       const OpIter &end,
                       Env &env,
                       Ops &out,
                       Opts &opts) const {
    auto &p(in->pos);
    auto &body(in->args[0]);
    if (body.type != &cx.expr_type) { throw ESys(p, "Invalid body: ", body); }
    Env body_env(cx, env);
    auto &body_opts(body.as_expr->opts);
    body.compile(p, body_env, body_opts);
    for (auto &eid: body_opts.ext_ids) { eid.src_reg = opts.get_reg(p, eid.id); }
    out.push_back(*in);
  }

  void DoType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &body(*op.args[0].as_expr);
    auto &opts(body.opts);
    Reg *eval_regs(cx.alloc_regs(opts.regs.size()));
    for (auto &eid: opts.ext_ids) { eval_regs[eid.dst_reg] = regs[eid.src_reg]; }
    cx.eval(body.ops, env, eval_regs);
  }

  void DoType::mark_refs(Op &op) const { op.args[0].mark_refs(); }

  void DoType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing body"); }
    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *body);
  }
}
