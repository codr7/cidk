#ifndef CIDK_OPS_HPP
#define CIDK_OPS_HPP

#include <vector>

#include "cidk/sym.hpp"

namespace cidk {
  using namespace std;
  
  struct Op;
  using Ops = vector<Op>;

  void get_ids(const Ops &in, IdSet &out);
  void mark_refs(Ops &ops);
}

#endif
