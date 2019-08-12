#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/expr.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/ops/do_env.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"

namespace cidk::ops {
  struct DoEnvData {
    Val body;
    Opts body_opts;
    
    DoEnvData(const Val &body): body(body) {}
  };
  
  const DoEnvType DoEnv("do-env");

  DoEnvType::DoEnvType(const string &id): OpType(id) {}

  void DoEnvType::init(Cx &cx, Op &op, const Val &body) const {
    op.data = DoEnvData(body);
  }

  void DoEnvType::compile(Cx &cx,
                          OpIter &in,
                          const OpIter &end,
                          Env &env,
                          Ops &out,
                          Opts &opts) const {
    auto &d(in->as<DoEnvData>());
    Env body_env(cx, env);
    d.body.compile(in->pos, body_env, d.body_opts);
    out.push_back(*in);
  }

  void DoEnvType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &d(op.as<DoEnvData>());
    cx.eval(d.body.as_expr->ops, env, d.body_opts, regs);
  }

  void DoEnvType::mark_refs(Op &op) const {
    auto &d(op.as<DoEnvData>());
    d.body.mark_refs();
    d.body_opts.mark_refs();
  }

  void DoEnvType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing body"); }

    if (body->type != &cx.expr_type) {
      throw ESys(p, "Invalid body: ", body->type->id);
    }

    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *body);
  }
}
