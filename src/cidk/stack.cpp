#include "cidk/e.hpp"
#include "cidk/stack.hpp"
#include "cidk/val.hpp"

namespace cidk {
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
