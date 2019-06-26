#include "cidk/cx.hpp"
#include "cidk/ops/swap.hpp"

namespace cidk::ops {
  const SwapType Swap("swap");

  SwapType::SwapType(string id): OpType(id) { }

  void SwapType::eval(Cx &cx, const Op &op) const {
    Stack &s(cx.stack);
    auto i(s.end());
    iter_swap(i-1, i);
  }

  void init(Op &op, const ops::SwapType &type) { }
}
