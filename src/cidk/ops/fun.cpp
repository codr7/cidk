#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/list.hpp"
#include "cidk/ops/fun.hpp"

namespace cidk::ops {
  const FunType Fun("fun");

  FunType::FunType(string id): OpType(id) { }

  void FunType::init(Op &op, cidk::Fun *fun) const { op.data = fun; }

  void FunType::eval(const Op &op, Env &env) const {
    auto &cx(env.cx);
    Pos p(op.pos);
    auto f(op.as<cidk::Fun *>());
    if (f->id) { env.set(p, f->id, Val(p, cx.fun_type, f), false); }
    f->env = env;
  }

  void FunType::read(Cx &cx, const Pos &pos, Reader &in, Env &env, Ops &out) const {
    auto p(pos);

    auto id(in.read_val(env));
    if (!id) { throw ERead(p, "Missing fun id"); }

    auto args(in.read_val(env));
    if (!args) { throw ERead(p, "Missing fun args"); }

    auto rets(in.read_val(env));
    if (!rets) { throw ERead(p, "Missing fun rets"); }

    cidk::Fun *f(cx.fun_type.pool.get(cx, pos, 
                                      id->as_sym,
                                      args->as_list->items,
                                      rets->as_list->items));

    env.set(p, f->id, Val(p, cx.fun_type, f), false);
    Env &body_env(*cx.env_pool.get(env));
    auto body(in.read_val(body_env));
    if (!body) { throw ERead(p, "Missing fun body"); }
    in.read_eop(env);
    f->body = *body;
    out.emplace_back(p, *this, f);
  }
}
