#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/list.hpp"
#include "cidk/ops/fun.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const FunType Fun("fun");

  FunType::FunType(string id): OpType(id) { }

  void FunType::init(Op &op, cidk::Fun *fun) const { op.data = fun; }

  void FunType::eval(const Op &op, Env &env, Stack &stack) const {
    auto &cx(env.cx);
    const Pos &p(op.pos);
    auto f(op.as<cidk::Fun *>());
    f->env.use(p, env, f->body_ids);
    if (f->id) { env.set(p, f->id, Val(p, cx.fun_type, f), false); }
  }

  void FunType::get_ids(const Op &op, IdSet &out) const {
    IdSet &bs(op.as<cidk::Fun *>()->body_ids);
    out.insert(bs.begin(), bs.end());
  }

  void FunType::read(Cx &cx,
                     Pos &pos,
                     istream &in,
                     Env &env,
                     Stack &stack,
                     Ops &out) const {
    Pos p(pos);

    auto id(read_val(pos, in, env, stack));
    if (!id) { throw ESys(p, "Missing fun id"); }

    auto args(read_val(pos, in, env, stack));
    if (!args) { throw ESys(p, "Missing fun args"); }

    auto rets(read_val(pos, in, env, stack));
    if (!rets) { throw ESys(p, "Missing fun rets"); }

    cidk::Fun *f(cx.fun_type.pool.get(cx, pos, 
                                      id->as_sym,
                                      args->as_list->items,
                                      rets->as_list->items));

    env.set(p, f->id, Val(p, cx.fun_type, f), false);
    Env &body_env(*cx.env_pool.get(env));
    auto body(read_val(pos, in, body_env, stack));
    if (!body) { throw ESys(p, "Missing fun body"); }
    read_eop(pos, in, env, stack);
    f->body = *body;
    f->body.get_ids(f->body_ids);
    f->env.use(p, env, f->body_ids);
    out.emplace_back(p, *this, f);
  }
}
