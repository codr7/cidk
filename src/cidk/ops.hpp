#ifndef CIDK_OPS_HPP
#define CIDK_OPS_HPP

#include <functional>
#include <vector>

#include "cidk/sym.hpp"

namespace cidk {
  using namespace std;
  
  struct Op;

  using Ops = vector<Op>;
  using OpIter = Ops::iterator;
  
  void init_op_types(Cx &cx);
  bool find_op(Ops &ops, function<bool (Ops &, OpIter &)> pred);
  void mark_refs(Ops &ops);
}

#endif
