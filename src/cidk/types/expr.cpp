#include "cidk/cx.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/val.hpp"

namespace cidk {
  ExprType::ExprType(Cx &cx,
                     const Pos &pos,
                     Env &env,
                     const Sym *id,
                     const vector<Type *> &parents):
    TValType<Expr *>(cx, pos, env, id, parents) {}

  void ExprType::compile(const Pos &pos, Val &val, Env &env, Opts &opts) const {
    cx.compile(val.as_expr->ops, opts, env);
  }

  void ExprType::dump(const Val &val, ostream &out) const {    
    out << val.as_expr << ":Expr";
  }

  bool ExprType::eval(const Pos &pos, const Val &val, Env &env, Reg *regs) const {
    return cx.eval(val.as_expr->ops, env, regs);
  }

  bool ExprType::find_op(const Val &val,
                         function<bool (Ops &, OpIter &)> pred) const {
    return cidk::find_op(val.as_expr->ops, pred);
  }

  bool ExprType::is(const Val &x, const Val &y) const {
    return x.as_expr == y.as_expr;
  }

  void ExprType::mark_refs(const Val &val) const { val.as_expr->mark_refs(); }

  void ExprType::set(Val &dst, Expr *val) const { dst.as_expr = val; }

  void ExprType::sweep(const Pos &pos, Val &val) { val.as_expr->sweep(cx, pos); }
}
