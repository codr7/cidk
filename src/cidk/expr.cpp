#include "cidk/cx.hpp"
#include "cidk/expr.hpp"
#include "cidk/types/expr.hpp"

namespace cidk {
  Expr::Expr(Cx &cx): Ref(cx) { }

  void Expr::sweep(const Pos &pos) { cx.expr_type.pool.put(this); }
}
