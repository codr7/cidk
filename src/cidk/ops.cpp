#include "cidk/op.hpp"
#include "cidk/ops.hpp"

namespace cidk {
  void get_ids(const Ops &in, IdSet &out) {
    for (auto &op: in) { op.get_ids(out); }
  }
  
  void mark_refs(Ops &in) {
    for (Op &o: in) { o.mark_refs(); }
  }
}
