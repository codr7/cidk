#include "cidk/cx.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/val.hpp"

namespace cidk {
  ExprType::ExprType(Cx &cx, const Pos &pos, const Sym *id):
    ValTType<Expr *>(cx, pos, id) { }

  void ExprType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << "{\n";
    for (auto &op: val.as_expr->body) { out << op.type->id << ";\n"; }
    out << "}\n";
  }

  void ExprType::dup(const Pos &pos, Val &dst, const Val &src) const {
    dst.as_expr = src.as_expr;
  }

  void ExprType::eval(const Val &val, Env &env) const {
    cx.eval(val.as_expr->body, env);
  }

  bool ExprType::is(const Pos &pos, const Val &x, const Val &y) const {
    return x.as_expr == y.as_expr;
  }

  void ExprType::mark_refs(const Pos &pos, const Val &val) {
    Expr &e(*val.as_expr);
    e.ref_state = RefState::mark;
  }

  void ExprType::set(const Pos &pos, Val &dst, Expr *val) const {
    dst.as_expr = val;
  }

  void ExprType::sweep(const Pos &pos, Val &val) {
    val.as_expr->sweep(pos);
  }
}
