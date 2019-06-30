#ifndef CIDK_STACK_HPP
#define CIDK_STACK_HPP

#include <optional>
#include <vector>

namespace cidk {
  using namespace std;

  struct Val;
  
  using Stack = vector<Val>;

  optional<Val> pop(const Pos &pos, Stack &s, bool silent);
}

#endif
