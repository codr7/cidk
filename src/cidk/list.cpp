#include "cidk/cmp.hpp"
#include "cidk/cx.hpp"
#include "cidk/list.hpp"
#include "cidk/types/list.hpp"

namespace cidk {
  List::List(Cx &cx): Ref(cx) {}

  List::Iter List::bsearch(const Pos &pos, const Val &key, Cmp<Val> cmp) {
    Iter beg(items.begin());
    size_t min(0), max(items.size());
    
    while (min < max) {
      size_t i((min + max) / 2);
      Iter it(beg + i);
      
      switch (cmp(pos, key, *it)) {
      case -1:
        max = i;
        break;
      case 1:
        min = i + 1;
        break;
      default:
        return it;
      }
    }

    return beg + max;
  }

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
