#include "cidk/cx.hpp"
#include "cidk/ops/swap.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const SwapType Swap("swap");

  SwapType::SwapType(const string &id): OpType(id) { }

  void SwapType::init(Op &op) const { }

  void SwapType::eval(const Op &op, Env &env, Stack &stack) const {
    auto i(stack.end());
    iter_swap(i-1, i-2);
  }

  void SwapType::read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    read_eop(pos, in, env, stack);
    out.emplace_back(pos, *this);
  }
}
