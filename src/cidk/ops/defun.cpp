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

  DefunType::DefunType(const string &id): OpType(id) {}

  void DefunType::init(Cx &cx, Op &op, Fun *fun) const { op.data = fun; }

  void DefunType::eval(Op &op, Env &env, Stack &stack) const {
    auto &cx(env.cx);
    const Pos &p(op.pos);
    auto f(op.as<Fun *>());
    f->env.use(env, f->body_ids);
    if (f->id) { env.set(p, f->id, Val(p, cx.fun_type, f), false); }
  }

  void DefunType::get_ids(const Op &op, IdSet &out) const {
    IdSet &bs(op.as<Fun *>()->body_ids);
    out.insert(bs.begin(), bs.end());
  }

  void DefunType::mark_refs(Op &op) const { op.as<Fun *>()->mark(); }

  void DefunType::read(Cx &cx,
                       Pos &pos,
                       istream &in,
                       ReadState &state,
                       Env &env,
                       Stack &stack,
                       Ops &out) const {
    Pos p(pos);

    auto id(read_val(pos, in, state, env, stack));
    if (!id) { throw ESys(p, "Missing function id"); }

    auto args(read_val(pos, in, state, env, stack));
    if (!args) { throw ESys(p, "Missing argument list"); }

    if (args->type != &cx.list_type) {
      throw ESys(p, "Invalid argument list: ", args->type->id);
    }
    
    auto rets(read_val(pos, in, state, env, stack));
    if (!rets) { throw ESys(p, "Missing return list"); }

    if (rets->type != &cx.list_type) {
      throw ESys(p, "Invalid return list: ", args->type->id);
    }

    Fun &f(env.add_fun(pos, id->as_sym, {}, {}));
    f.args.parse(cx, p, *args->as_list, env, stack);
    Env &body_env(*cx.env_pool.get(env));
    f.body_opts.env_depth = 0;
    auto body(read_val(pos, in, f.body_opts, body_env, stack));
    if (!body) { throw ESys(p, "Missing function body"); }

    if (body->type != &cx.expr_type) {
      throw ESys(p, "Invalid function body: ", body->type->id);
    }
    
    state.merge(f.body_opts);
    if (!state.env_depth) { state.env_escape = true; }
    
    read_eop(pos, in, env, stack);    
    auto &as(f.args.items);

    for (auto i(as.rbegin()); i != as.rend(); i++) {
      if (i->id) {
        f.body.emplace_back(cx, pos, Let, i->id, cx.$);
        f.body_opts.env_extend = true;
      }
    }

    auto &b(body->as_expr->body);
    copy(b.begin(), b.end(), back_inserter(f.body));
    body->get_ids(f.body_ids);
    f.env.use(env, f.body_ids);
    out.emplace_back(cx, p, *this, &f);
  }
}
