#ifndef CIDK_STACK_HPP
#define CIDK_STACK_HPP

#include <optional>
#include <vector>

namespace cidk {
  using namespace std;

  struct Pos;
  struct Val;
  
  using Stack = vector<Val>;

  void mark_items(Stack &s); 
  const Val &pop(const Pos &pos, Stack &s);
  optional<Val> try_pop(Stack &s);

  ostream &operator <<(ostream &out, const Stack &s);
}

#endif
