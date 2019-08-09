#ifndef CIDK_FIMP_HPP
#define CIDK_FIMP_HPP

#include <functional>

namespace cidk {
  struct Cx;
  struct Env;
  struct Fun;
  struct Pos;
  
  using Fimp = function<void (Cx &, const Pos &, const Fun &, Env &)>;
}

#endif
