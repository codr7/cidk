#include "cidk/e.hpp"
#include "cidk/stack.hpp"
#include "cidk/val.hpp"

namespace cidk {
  void mark_items(Stack &s) {
    for (Val &v: s) { v.mark_refs(); }
  }

  Val pop(const Pos &pos, Stack &s) {
    if (s.empty()) { throw ESys(pos, "Stack is empty"); }
    Val out(s.back());
    s.pop_back();
    return out;
  }

  optional<Val> try_pop(Stack &s) {
    if (s.empty()) { return {}; }
    Val out(s.back());
    s.pop_back();
    return out;
  }
}
