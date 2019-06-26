#ifndef CIDK_OP_SWAP_HPP
#define CIDK_OP_SWAP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct SwapType: OpType {
    SwapType(string id);
    virtual void eval(Cx &cx, const Op &op) const override;
  };

  extern const SwapType Swap;

  void init(Op &op, const ops::SwapType &type);
}

#endif
