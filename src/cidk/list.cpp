#include "cidk/cmp.hpp"
#include "cidk/cx.hpp"
#include "cidk/list.hpp"
#include "cidk/types/list.hpp"

namespace cidk {
  List::List(Cx &cx): Ref(cx) {}

  void List::mark() {
    if (!ref_mark) {
      ref_mark = true;
      for (auto &v: items) { v.mark_refs(); }
    }
  }

  void List::sort(const Pos &pos, Cmp<Val> cmp) {
    std::sort(items.begin(), items.end(),
              [&](const Val &x, const Val &y) { return cmp(pos, x, y); });
  }
  
  void List::sweep(Cx &cx, const Pos &pos) { cx.list_type.pool.put(this); }
}
