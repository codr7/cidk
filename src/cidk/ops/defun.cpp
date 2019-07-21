#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/list.hpp"
#include "cidk/ops/defun.hpp"
#include "cidk/ops/let.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/list.hpp"

namespace cidk::ops {
  struct DefunData {
    const Sym *id;
    Val args, body;
    Fun *fun;

    DefunData(const Sym *id, const Val &args, const Val &body):
      id(id), args(args), body(body), fun(nullptr) {}
  };
  
  const DefunType Defun("defun");

  DefunType::DefunType(const string &id): OpType(id) {}

  void DefunType::init(Cx &cx,
                       Op &op,
                       const Sym *id,
                       const Val &args,
                       const Val &body) const {
    op.data = DefunData(id, args, body);
  }

  void DefunType::compile(Cx &cx,
                          Op &op,
                          Env &env,
                          Stack &stack,
                          Ops &out,
                          Opts *opts) const {
    const Pos &p(op.pos);
    auto &d(op.as<DefunData>());
    Fun &f(env.add_fun(p, d.id, {}, {}));

    f.args.parse(cx, p, *d.args.as_list, env, stack);
    auto &as(f.args.items);

    for (auto i(as.rbegin()); i != as.rend(); i++) {
      if (i->id) { f.body.emplace_back(cx, p, Let, i->id, cx.$); }
    }

    auto &b(d.body.as_expr->body);
    copy(b.begin(), b.end(), back_inserter(f.body));
    d.body.get_ids(f.body_ids);
    f.env.use(env, f.body_ids);
    cx.compile(f.body, &f.body_opts, *cx.env_pool.get(env), stack);

    if (opts) {
      opts->merge(f.body_opts);
      opts->env_escape = true;
    }
    
    d.fun = &f;
    out.push_back(op);
  }
  
  void DefunType::eval(Op &op, Env &env, Stack &stack) const {
    auto &f(*op.as<DefunData>().fun);
    f.env.use(env, f.body_ids);
  }

  void DefunType::get_ids(const Op &op, IdSet &out) const {
    IdSet &ids(op.as<DefunData>().fun->body_ids);
    out.insert(ids.begin(), ids.end());
  }

  void DefunType::mark_refs(Op &op) const { op.as<DefunData>().fun->mark(); }

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

    Env &body_env(*cx.env_pool.get(env));
    auto body(read_val(pos, in, body_env, stack));
    if (!body) { throw ESys(p, "Missing function body"); }

    if (body->type != &cx.expr_type) {
      throw ESys(p, "Invalid function body: ", body->type->id);
    }

    read_eop(pos, in, env, stack);    
    out.emplace_back(cx, p, *this, id->as_sym, *args, *body);
  }
}
