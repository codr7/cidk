#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/list.hpp"
#include "cidk/ops/fun.hpp"

namespace cidk::ops {
  const FunType Fun("fun");

  FunType::FunType(string id): OpType(id) { }

  void FunType::init(Op &op, cidk::Fun *fun) const { op.data = fun; }

  void FunType::eval(const Op &op, Env &env) const {
    auto f(op.as<cidk::Fun *>());
    f->env.merge(env);
  }

  void FunType::read(Cx &cx, const Pos &pos, Reader &in, Env &env, Ops &out) const {
    auto p(pos);

    auto id(in.read_val(env));
    if (!id) { throw ERead(p, "Missing fun id"); }

    auto args(in.read_val(env));
    if (!args) { throw ERead(p, "Missing fun args"); }

    auto rets(in.read_val(env));
    if (!rets) { throw ERead(p, "Missing fun rets"); }

    cidk::Fun *f(cx.fun_type.pool.alloc());
    env.set(p, id->as_sym, Val(p, cx.fun_type, f), false);

    new (f) cidk::Fun(cx, pos, 
                      id->as_sym,
                      args->as_list->items,
                      rets->as_list->items,
                      env);
      
    auto body(in.read_val(f->env));
    if (!body) { throw ERead(p, "Missing fun body"); }
    in.read_eop(env);
    f->body = *body;
    out.emplace_back(p, *this, f);
  }
}
