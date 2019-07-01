#include "cidk/cx.hpp"
#include "cidk/ops/swap.hpp"

namespace cidk::ops {
  const SwapType Swap("swap");

  SwapType::SwapType(string id): OpType(id) { }

  void SwapType::init(Op &op) const { }

  void SwapType::eval(const Op &op, Env &env) const {
    Stack &s(env.cx.stack);
    auto i(s.end());
    iter_swap(i-1, i);
  }

  void SwapType::read(Cx &cx, const Pos &pos, Reader &in, Env &env, Ops &out) const {
    in.read_eop(env);
    out.emplace_back(pos, *this);
  }
}
