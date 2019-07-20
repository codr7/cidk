#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/do_env.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/env.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  struct DoEnvData {
    Val in, body;
    DoEnvData(const Val &in, const Val &body): in(in), body(body) {}
  };
  
  const DoEnvType DoEnv("do-env");

  DoEnvType::DoEnvType(const string &id): OpType(id) {}

  void DoEnvType::init(Cx &cx, Op &op, const Val &in, const Val &body) const {
    op.data = DoEnvData(in, body);
  }

  void DoEnvType::compile(Cx &cx,
                          Op &op,
                          Env &env,
                          Stack &stack,
                          Ops &out,
                          Opts *opts) const {
    auto &d(op.as<DoEnvData>());
    d.in.compile(cx, op.pos, env, stack, opts);
    Env *de(&env);
    
    if (d.in.type == &cx.nil_type) {
      de = env.cx.env_pool.get(cx);
    } else if (d.in.type == &cx.bool_type && d.in.as_bool) {
      de = env.cx.env_pool.get(env);
    }

    d.body.compile(cx, op.pos, *de, stack, opts);
    out.push_back(op);
  }

  void DoEnvType::eval(Op &op, Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    const Pos &p(op.pos);
    const DoEnvData &d(op.as<DoEnvData>());
    Env *de(nullptr);

    if (d.in.type == &cx.nil_type) {
      de = env.cx.env_pool.get(cx);
    } else if (d.in.type == &cx.bool_type && d.in.as_bool) {
      de = env.cx.env_pool.get(env);
    } else {
      d.in.eval(p, env, stack);
      auto in(pop(p, stack));
      de = &in.get_env();
    }
    
    d.body.eval(p, *de, stack);
  }

  void DoEnvType::get_ids(const Op &op, IdSet &out) const {
    DoEnvData d(op.as<DoEnvData>());
    d.in.get_ids(out);
    d.body.get_ids(out);
  }

  void DoEnvType::mark_refs(Op &op) const {
    auto &d(op.as<DoEnvData>());
    d.in.mark_refs();
    d.body.mark_refs();
  }

  void DoEnvType::read(Cx &cx, Pos &pos,
                       istream &in,
                       Env &env,
                       Stack &stack,
                       Ops &out) const {
    Pos p(pos);
    auto _in(read_val(pos, in, env, stack));
    if (!_in) { throw ESys(p, "Missing do-env input"); }

    auto body(read_val(pos, in, *env.cx.env_pool.get(env), stack));
    if (!body) { throw ESys(p, "Missing do-env body"); }

    if (body->type != &cx.expr_type) {
      throw ESys(p, "Expected Expr, was: ", body->type->id);
    }
    
    read_eop(pos, in, env, stack);
    out.emplace_back(cx, p, *this, *_in, *body);
  }
}
