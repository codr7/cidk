#include "cidk/cx.hpp"
#include "cidk/list.hpp"
#include "cidk/types/list.hpp"

namespace cidk {
  List::List(Cx &cx): Ref(cx) { }

  void List::sweep(const Pos &pos) {
    cx.list_type.pool.put(this);
  }
}
