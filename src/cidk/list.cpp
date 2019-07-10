#include "cidk/cx.hpp"
#include "cidk/list.hpp"
#include "cidk/types/list.hpp"

namespace cidk {
  List::List(Cx &cx): Ref(cx) {}

  void List::mark() {
    if (!is_marked) {
      is_marked = true;
      for (auto &v: items) { v.mark_refs(); }
    }
  }

  void List::sweep(Cx &cx, const Pos &pos) { cx.list_type.pool.put(this); }
}
