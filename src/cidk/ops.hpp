#ifndef CIDK_OPS_HPP
#define CIDK_OPS_HPP

#include <vector>

namespace cidk {
  using namespace std;
  
  struct Op;
  using Ops = vector<Op>;

  void mark_refs(Ops &ops);
}

#endif
