#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/list.hpp"
#include "cidk/ops/defun.hpp"
#include "cidk/ops/let.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/list.hpp"

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
    if (!args) { throw ESys(p, "Missing argument list"); }

    if (args->type != &cx.list_type) {
      throw ESys(p, "Invalid argument list: ", args->type->id);
    }
    
    auto rets(read_val(pos, in, env, stack));
    if (!rets) { throw ESys(p, "Missing return list"); }

    if (rets->type != &cx.list_type) {
      throw ESys(p, "Invalid return list: ", args->type->id);
    }

    Fun &f(env.add_fun(pos, id->as_sym, {}, {}));
    Env &body_env(*cx.env_pool.get(env));
    auto body(read_val(pos, in, body_env, stack));
    if (!body) { throw ESys(p, "Missing function body"); }

    if (body->type != &cx.expr_type) {
      throw ESys(p, "Invalid function body: ", body->type->id);
    }
    
    read_eop(pos, in, env, stack);    
    auto &as(f.args.items);

    for (auto i(as.rbegin()); i != as.rend(); i++) {
      f.body.emplace_back(pos, Let, i->id, cx.S);
    }

    auto &b(body->as_expr->body);
    copy(b.begin(), b.end(), back_inserter(f.body));
    body->get_ids(f.body_ids);
    f.env.use(p, env, f.body_ids);
    out.emplace_back(p, *this, &f);
  }
}
