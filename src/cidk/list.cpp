#include "cidk/cx.hpp"
#include "cidk/list.hpp"
#include "cidk/types/list.hpp"

namespace cidk {
  List::List(Cx &cx): Ref(cx) { }

  void List::mark(const Pos &pos) {
    if (!is_marked) {
      is_marked = true;
      for (auto &v: items) { v.mark_refs(pos); }
    }
  }

  void List::sweep(const Pos &pos) { cx.list_type.pool.put(this); }
}
