#include "cidk/op.hpp"
#include "cidk/ops.hpp"

namespace cidk {
  void mark_refs(Ops &in) {
    for (Op &o: in) { o.mark_refs(); }
  }
}
