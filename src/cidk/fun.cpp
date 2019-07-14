#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/expr.hpp"
#include "cidk/fun.hpp"

namespace cidk {
  void Fun::mark() {
    if (!ref_marked) {
      ref_marked = true;
      env.mark();
      mark_refs(body);
    }

    if (ref_state == RefState::sweep) { ref_state = RefState::keep; }
  }  

  void Fun::sweep(Cx &cx, const Pos &pos) { cx.fun_type.pool.put(this); }
}
