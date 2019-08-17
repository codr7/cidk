#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/expr.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/ops/do_env.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"

namespace cidk::ops {
  const DoEnvType DoEnv("do-env");

  DoEnvType::DoEnvType(const string &id): OpType(id) {}

  void DoEnvType::init(Cx &cx, Op &op, const Val &body) const { op.args[0] = body; }

  void DoEnvType::compile(Cx &cx,
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
    for (auto &r: body_opts.ext_ids) { r.val = env.get(p, r.id); }
    out.push_back(*in);
  }

  void DoEnvType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &body(*op.args[0].as_expr);
    cx.eval(body.ops, env, body.opts, regs);
  }

  void DoEnvType::mark_refs(Op &op) const { op.args[0].mark_refs(); }

  void DoEnvType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing body"); }
    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *body);
  }
}
