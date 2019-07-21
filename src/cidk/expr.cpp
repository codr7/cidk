#include "cidk/cx.hpp"
#include "cidk/expr.hpp"
#include "cidk/types/expr.hpp"

namespace cidk {
  Expr::Expr(Cx &cx): Ref(cx) {}

  Expr::Expr(Cx &cx, const Ops &body): Ref(cx), body(body) {}

  void Expr::eval(Cx &cx, Env &env, Stack &stack) { cx.eval(body, env, stack); }

  void Expr::mark() {
    if (!ref_mark) {
      ref_mark = true;
      mark_refs(body);
    }
  }
  
  void Expr::sweep(Cx &cx, const Pos &pos) { cx.expr_type.pool.put(this); }
}
