#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/list.hpp"
#include "cidk/ops/defun.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"

namespace cidk::ops {
  const DefunType Defun("defun");

  DefunType::DefunType(const string &id): OpType(id) { }

  void DefunType::init(Op &op, Fun *fun) const { op.data = fun; }

  void DefunType::eval(const Op &op, Env &env, Stack &stack) const {
    auto &cx(env.cx);
    const Pos &p(op.pos);
    auto f(op.as<Fun *>());
    f->env.use(p, env, f->body_ids);
    if (f->id) { env.set(p, f->id, Val(p, cx.fun_type, f), false); }
  }

  void DefunType::get_ids(const Op &op, IdSet &out) const {
    IdSet &bs(op.as<Fun *>()->body_ids);
    out.insert(bs.begin(), bs.end());
  }

  void DefunType::read(Cx &cx,
                       Pos &pos,
                       istream &in,
                       Env &env,
                       Stack &stack,
                       Ops &out) const {
    Pos p(pos);

    auto id(read_val(pos, in, env, stack));
    if (!id) { throw ESys(p, "Missing function id"); }

    auto args(read_val(pos, in, env, stack));
    if (!args) { throw ESys(p, "Missing function args"); }

    auto rets(read_val(pos, in, env, stack));
    if (!rets) { throw ESys(p, "Missing funtion returns"); }

    Fun *f(cx.fun_type.pool.get(cx, pos, 
                                id->as_sym,
                                args->as_list->items,
                                rets->as_list->items));

    env.set(p, f->id, Val(p, cx.fun_type, f), false);
    Env &body_env(*cx.env_pool.get(env));
    auto body(read_val(pos, in, body_env, stack));
    if (!body) { throw ESys(p, "Missing function body"); }

    if (body->type != &cx.expr_type) {
      throw ESys(p, "Invalid function body: ", body->type->id);
    }
    
    read_eop(pos, in, env, stack);
    f->body = body->as_expr;
    body->get_ids(f->body_ids);
    f->env.use(p, env, f->body_ids);
    out.emplace_back(p, *this, f);
  }
}
