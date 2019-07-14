#include "cidk/cx.hpp"
#include "cidk/list.hpp"
#include "cidk/types/list.hpp"

namespace cidk {
  List::List(Cx &cx): Ref(cx) {}

  void List::mark() {
    if (!ref_marked) {
      ref_marked = true;
      for (auto &v: items) { v.mark_refs(); }
    }
    
    if (ref_state == RefState::sweep) { ref_state = RefState::keep; }
  }

  void List::sweep(Cx &cx, const Pos &pos) { cx.list_type.pool.put(this); }
}
