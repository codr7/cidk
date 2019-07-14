#include "cidk/cx.hpp"
#include "cidk/expr.hpp"
#include "cidk/types/expr.hpp"

namespace cidk {
  Expr::Expr(Cx &cx): Ref(cx) {}

  void Expr::eval(Env &env, Stack &stack) { env.cx.eval(body, env, stack); }

  void Expr::mark() {
    if (!ref_mark) {
      ref_mark = true;
      mark_refs(body);
    }
  }
  
  void Expr::sweep(Cx &cx, const Pos &pos) { cx.expr_type.pool.put(this); }
}
