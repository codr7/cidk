#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/expr.hpp"
#include "cidk/ops/for.hpp"
#include "cidk/read.hpp"
#include "cidk/stack.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  struct ForData {
    Val src;
    const Sym *var;
    Expr &body;
    
    ForData(const Val &src, const Sym *var, Expr &body):
      src(src), var(var), body(body) {}
  };

  const ForType For("for");

  ForType::ForType(const string &id): OpType(id) {}
  
  void ForType::init(Cx &cx,
                    Op &op,
                    const Val &src,
                    const Sym *var,
                    Expr &body) const {
    op.data = ForData(src, var, body);
  }

  void ForType::compile(Cx &cx,
                       OpIter &in,
                       const OpIter &end,
                       Env &env,
                       Stack &stack,
                       Ops &out,
                       Opts *opts) const {
    auto &d(in->as<ForData>());
    d.src.compile(cx, in->pos, env, stack, opts);
    cx.compile(d.body.ops, opts, env, stack);
    out.push_back(*in);
  }

  void ForType::eval(Cx &cx, Op &op, Env &env, Stack &stack) const {
    auto &p(op.pos);
    auto &d(op.as<ForData>());
    d.src.eval(cx, p, env, stack);
    Val src(pop(p, stack));
    
    for (Int i(0); i < src.as_int; i++) {
      stack.emplace_back(cx.int_type, i);
      cx.eval(d.body.ops, env, stack);
    }
  }

  void ForType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<ForData>());
    d.src.get_ids(out);
    cidk::get_ids(d.body.ops, out);
  }

  void ForType::mark_refs(Op &op) const {
    auto &d(op.as<ForData>());
    d.src.mark_refs();
    d.body.mark();
  }

  void ForType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);

    auto src(read_val(cx, pos, in));
    if (!src || src->is_eop()) { throw ESys(p, "Missing for source"); }

    auto var(read_val(cx, pos, in));
    if (!var || var->is_eop()) { throw ESys(p, "Missing for var"); }

    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing for body"); }

    if (!body->type->isa(cx.expr_type)) {
      throw ESys(p, "Invalid for body, expected Expr: ", body->type->id);
    }
    
    read_eop(pos, in);

    out.emplace_back(cx, p, *this,
                     *src,
                     (var->type == &cx.sym_type) ? var->as_sym : nullptr,
                     *body->as_expr);
  }
}
