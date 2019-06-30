#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/fun.hpp"

namespace cidk {
  void Fun::sweep(const Pos &pos) { cx.fun_type.pool.put(this); }
}
