#include "cidk/e.hpp"
#include "cidk/stack.hpp"
#include "cidk/val.hpp"

namespace cidk {
  void mark_items(Pos &pos, Stack &s) {
    for (Val &v: s) { v.mark_refs(pos); }
  }

  optional<Val> pop(const Pos &pos, Stack &s, bool silent) {
    if (s.empty()) {
      if (!silent) { throw ESys(pos, "Stack is empty"); }
      return {};
    }
    
    Val out(s.back());
    s.pop_back();
    return out;
  }
}
