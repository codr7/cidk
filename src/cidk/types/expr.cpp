#include "cidk/cx.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/val.hpp"

namespace cidk {
  ExprType::ExprType(Cx &cx,
                     const Pos &pos,
                     const Sym *id,
                     const vector<Type *> &parents):
    TValType<Expr *>(cx, pos, id, parents) {}

  void ExprType::cp(Val &dst, const Val &src) const { dst.as_expr = src.as_expr; }

  void ExprType::compile(Cx &cx,
                         const Pos &pos,
                         Val &val,
                         Env &env,
                         Stack &stack,
                         Opts &opts) const {
    cx.compile(val.as_expr->ops, opts, env, stack);
  }

  void ExprType::dump(const Val &val, ostream &out) const {    
    out << "expr:" << val.as_expr;
  }

  void ExprType::eval(Cx &cx,
                      const Pos &pos,
                      const Val &val,
                      Env &env,
                      Regs &regs,
                      Stack &stack) const {
    cx.eval(val.as_expr->ops, env, regs, stack);
  }

  void ExprType::get_ids(const Val &val, IdSet &out) const {
    cidk::get_ids(val.as_expr->ops, out);
  }

  bool ExprType::is(const Val &x, const Val &y) const {
    return x.as_expr == y.as_expr;
  }

  void ExprType::mark_refs(const Val &val) const { val.as_expr->mark(); }

  void ExprType::set(Val &dst, Expr *val) const { dst.as_expr = val; }

  void ExprType::sweep(const Pos &pos, Val &val) { val.as_expr->sweep(cx, pos); }
}
