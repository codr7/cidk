#include "cidk/cx.hpp"
#include "cidk/expr.hpp"
#include "cidk/types/expr.hpp"

namespace cidk {
  Expr::Expr(Cx &cx): Ref(cx) { }

  void Expr::eval(Env &env, Stack &stack) const { cx.eval(body, env, stack); }

  void Expr::mark() {
    is_marked = true;
    mark_refs(body);
  }
  
  void Expr::sweep(const Pos &pos) { cx.expr_type.pool.put(this); }
}
