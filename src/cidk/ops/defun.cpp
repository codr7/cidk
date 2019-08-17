#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/list.hpp"
#include "cidk/ops/defun.hpp"
#include "cidk/ops/let.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/fun.hpp"
#include "cidk/types/list.hpp"

namespace cidk::ops {
  const DefunType Defun("defun");

  DefunType::DefunType(const string &id): OpType(id) {}

  void DefunType::init(Cx &cx,
                       Op &op,
                       const Val &id,
                       const Val &args,
                       const Val &rets,
                       const Val &body) const {
    op.args[0] = id;
    op.args[1] = args;
    op.args[2] = rets;
    op.args[3] = body;
  }

  void DefunType::compile(Cx &cx,
                          OpIter &in,
                          const OpIter &end,
                          Env &env,
                          Ops &out,
                          Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);

    auto &id(args[0]);
    if (id.type != &cx.sym_type) { throw ESys(p, "Invalid id: ", id); }

    auto &body(args[3]);
    if (body.type != &cx.expr_type) { throw ESys(p, "Invalid body: ", body); }
    
    if (args[1].type != &cx.list_type) {
      throw ESys(p, "Invalid argument list: ", args[1]);
    }

    if (args[2].type != &cx.list_type) {
      throw ESys(p, "Invalid return list: ", args[2]);
    }

    Fun &f(*cx.fun_type.pool.get(cx, p, env, id.as_sym));
    f.args.parse(cx, p, *args[1].as_list, env);
    auto &as(f.args.items);

    for (auto i(as.rbegin()); i != as.rend(); i++) {
      if (i->id) { f.body.emplace_back(cx, p, Let, Val(cx.sym_type, i->id), cx.$); }
    }

    f.init(cx, p, env);
    Val fv(cx.fun_type, &f);
    env.let(cx, p, f.id, fv);
    f.env.let(cx, p, f.id, fv);
    
    auto &bops(body.as_expr->ops);
    copy(bops.begin(), bops.end(), back_inserter(f.body));
    cx.compile(f.body, f.body_opts, f.env);
    for (auto &r: f.body_opts.ext_ids) { r.src_reg = opts.get_reg(p, r.id); }
    in->args[4].reset(cx.fun_type, &f);
    out.push_back(*in);
  }
  
  void DefunType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    for (auto &r: op.args[4].as_fun->body_opts.ext_ids) { r.val = regs[r.src_reg]; }
  }

  void DefunType::mark_refs(Op &op) const {
    auto &args(op.args);

    for (int i(1); i < 5; i++) {
      if (args[i].type) { args[i].mark_refs(); }
    }
  }

  void DefunType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);

    auto id(read_val(cx, pos, in));
    if (!id || id->is_eop()) { throw ESys(p, "Missing id"); }

    auto args(read_val(cx, pos, in));
    if (!args || args->is_eop()) { throw ESys(p, "Missing argument list"); }

    auto rets(read_val(cx, pos, in));
    if (!rets || rets->is_eop()) { throw ESys(p, "Missing return list"); }

    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing body"); }

    read_eop(pos, in);    
    out.emplace_back(cx, p, *this, *id, *args, *rets, *body);
  }
}
