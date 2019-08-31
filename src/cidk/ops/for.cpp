#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/for.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  const ForType For("for");

  ForType::ForType(const string &id): OpType(id) {}
  
  void ForType::init(Cx &cx,
                     Op &op,
                     const Val &src,
                     const Val &var,
                     const Val &body) const {
    op.args[0] = src;
    op.args[1] = var;
    op.args[2] = body;
  }

  void ForType::compile(Cx &cx,
                        OpIter &in,
                        const OpIter &end,
                        Env &env,
                        Ops &out,
                        Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);
    for (int i(0); i < 3; i++) { args[i].compile(p, env, opts); }
    auto &var(args[1]);

    if (var.type != &cx.bool_type && var.type != &cx.sym_type) {
      throw ESys(p, "Invalid var: ", var);
    }

    out.push_back(*in);
  }

  void ForType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);
    
    args[0].eval(p, env, regs);
    auto n(cx.pop(p).as_int);
    auto &var(args[1]), &body(args[2]);
    bool push(var.type == &cx.bool_type && var.as_bool);
    
    for (Int i(0); i < n; i++) {
      if (push) { cx.push(p, cx.int_type, i); }
      body.eval(p, env, regs);
    }
  }

  bool ForType::find_op(Op &op, function<bool (Ops &, OpIter &)> pred) const {
    auto &args(op.args);

    for (int i(0); i < 3; i++) {
      if (args[i].find_op(pred)) { return true; }
    }

    return false;
  }

  void ForType::mark_refs(Op &op) const {
    auto &args(op.args);
    for (int i(0); i < 3; i++) { args[i].mark_refs(); }
  }

  void ForType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);

    auto src(read_val(cx, pos, in));
    if (!src || src->is_eop()) { throw ESys(p, "Missing for source"); }

    auto var(read_val(cx, pos, in));
    if (!var || var->is_eop()) { throw ESys(p, "Missing for var"); }

    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing for body"); }

    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *src, *var, *body);
  }
}
