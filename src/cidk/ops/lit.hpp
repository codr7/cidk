#ifndef CIDK_OP_LIT_HPP
#define CIDK_OP_LIT_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct LitType: OpType {
    LitType(string id);
    virtual void eval(Cx &cx, const Op &op) const override;
  };

  extern const LitType Lit;
}


namespace cidk {
  template <>
  inline void init(Op &op, const ops::LitType &type, const Val &val) {
    op.data = val;
  }
}

#endif
