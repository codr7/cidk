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
                          OpIter &in,
                          const OpIter &end,
                          Env &env,
                          Stack &stack,
                          Ops &out,
                          Opts *opts) const {
    const Pos &p(in->pos);
    auto &d(in->as<DefunData>());
    Fun &f(env.add_fun(cx, p, d.id, {}, {}));

    f.args.parse(cx, p, *d.args.as_list, env, stack);
    auto &as(f.args.items);

    for (auto i(as.rbegin()); i != as.rend(); i++) {
      if (i->id) { f.body.emplace_back(cx, p, Let, i->id, cx.$); }
    }

    auto &b(d.body.as_expr->body);
    copy(b.begin(), b.end(), back_inserter(f.body));
    d.body.get_ids(f.body_ids);
    f.env.use(cx, env, f.body_ids);
    cx.compile(f.body, &f.body_opts, *cx.env_pool.get(cx, env), stack);
    if (opts) { opts->env_escape = true; }
    d.fun = &f;
    out.push_back(*in);
  }
  
  void DefunType::eval(Cx &cx, Op &op, Env &env, Stack &stack) const {
    auto &f(*op.as<DefunData>().fun);
    f.env.use(cx, env, f.body_ids);
  }

  void DefunType::get_ids(const Op &op, IdSet &out) const {
    IdSet &ids(op.as<DefunData>().fun->body_ids);
    out.insert(ids.begin(), ids.end());
  }

  void DefunType::mark_refs(Op &op) const { op.as<DefunData>().fun->mark(); }

  void DefunType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);

    auto id(read_val(cx, pos, in));
    if (!id || id->is_eop()) { throw ESys(p, "Missing function id"); }

    auto args(read_val(cx, pos, in));
    if (!args || args->is_eop()) { throw ESys(p, "Missing argument list"); }

    if (args->type != &cx.list_type) {
      throw ESys(p, "Invalid argument list: ", args->type->id);
    }
    
    auto rets(read_val(cx, pos, in));
    if (!rets || rets->is_eop()) { throw ESys(p, "Missing return list"); }

    if (rets->type != &cx.list_type) {
      throw ESys(p, "Invalid return list: ", args->type->id);
    }

    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing function body"); }

    if (!body->type->isa(&cx.expr_type)) {
      throw ESys(p, "Invalid function body: ", body->type->id);
    }

    read_eop(pos, in);    
    out.emplace_back(cx, p, *this, id->as_sym, *args, *body);
  }
}
