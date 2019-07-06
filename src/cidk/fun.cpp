#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/fun.hpp"

namespace cidk {
  void Fun::mark(const Pos &pos) {
    if (!is_marked) {
      is_marked = true;
      env.mark(pos);
      body.mark_refs(pos);
    }
  }  

  void Fun::sweep(const Pos &pos) { cx.fun_type.pool.put(this); }
}
