#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/fun.hpp"

namespace cidk {
  void Fun::mark(const Pos &pos) {
    if (ref_state == RefState::_) {
      ref_state = RefState::mark;
      env.ref_state = RefState::mark;
      env.mark_refs(pos);
    }
  }  

  void Fun::sweep(const Pos &pos) { cx.fun_type.pool.put(this); }
}
