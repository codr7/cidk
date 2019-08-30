#include "cidk/cx.hpp"
#include "cidk/expr.hpp"
#include "cidk/types/expr.hpp"

namespace cidk {
  Expr::Flag::Root Expr::FLAGS("Expr:Flag");
  const Expr::Flag Expr::INLINE(Expr::FLAGS, "INLINE");

  Expr::Expr(Cx &cx): Ref(cx), flags(FLAGS) {}

  Expr::Expr(Cx &cx, const Ops &ops): Ref(cx), ops(ops), flags(FLAGS) {}

  void Expr::mark() {
    if (!ref_mark) {
      ref_mark = true;
      mark_refs(ops);
      opts.mark_refs();
    }
  }
  
  void Expr::sweep(Cx &cx, const Pos &pos) { cx.expr_type.pool.put(this); }
}
