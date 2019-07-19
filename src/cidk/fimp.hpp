#ifndef CIDK_FIMP_HPP
#define CIDK_FIMP_HPP

#include <functional>
#include "cidk/stack.hpp"

namespace cidk {
  struct Cx;
  struct Env;
  struct Pos;
  
  using Fimp = function<void (Cx &, const Pos &, const Fun &, Env &, Stack &)>;
}

#endif
