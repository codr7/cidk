#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ext_id.hpp"
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
                          Ops &out,
                          Opts &opts) const {
    auto &p(in->pos);
    auto &d(in->as<DefunData>());
    Fun &f(env.add_fun(cx, p, d.id, {}, {}));

    f.args.parse(cx, p, *d.args.as_list, env);
    auto &as(f.args.items);

    for (auto i(as.rbegin()); i != as.rend(); i++) {
      if (i->id) { f.body.emplace_back(cx, p, Let, i->id, cx.$); }
    }

    auto &b(d.body.as_expr->ops);
    copy(b.begin(), b.end(), back_inserter(f.body));
    f.env.let(cx, p, f.id, Val(cx.fun_type, &f));
    cx.compile(f.body, f.body_opts, f.env);
    for (auto &r: f.body_opts.ext_ids) { r.src_reg = opts.get_reg(p, r.id); }
    d.fun = &f;
    out.push_back(*in);
  }
  
  void DefunType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &f(*op.as<DefunData>().fun);
    for (auto &r: f.body_opts.ext_ids) { r.val = regs[r.src_reg]; }
  }

  void DefunType::mark_refs(Op &op) const { op.as<DefunData>().fun->mark(); }

  void DefunType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);

    auto id(read_val(cx, pos, in));
    if (!id || id->is_eop()) { throw ESys(p, "Missing id"); }

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
    if (!body || body->is_eop()) { throw ESys(p, "Missing body"); }

    if (body->type != &cx.expr_type) {
      throw ESys(p, "Invalid body: ", body->type->id);
    }

    read_eop(pos, in);    
    out.emplace_back(cx, p, *this, id->as_sym, *args, *body);
  }
}
