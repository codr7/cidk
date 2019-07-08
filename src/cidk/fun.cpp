#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/expr.hpp"
#include "cidk/fun.hpp"

namespace cidk {
  void Fun::mark() {
    if (!is_marked) {
      is_marked = true;
      env.mark();
      body->mark();
    }
  }  

  void Fun::sweep(const Pos &pos) { cx.fun_type.pool.put(this); }
}
