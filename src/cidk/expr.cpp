#include "cidk/cx.hpp"
#include "cidk/expr.hpp"
#include "cidk/types/expr.hpp"

namespace cidk {
  Expr::Expr(Cx &cx): Ref(cx) {}

  void Expr::eval(Env &env, Stack &stack) { env.cx.eval(body, env, stack); }

  void Expr::mark() {
    if (!ref_marked) {
      ref_marked = true;
      mark_refs(body);
    }
    
    if (ref_state == RefState::sweep) { ref_state = RefState::keep; }
  }
  
  void Expr::sweep(Cx &cx, const Pos &pos) { cx.expr_type.pool.put(this); }
}
