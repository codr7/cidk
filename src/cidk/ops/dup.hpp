#ifndef CIDK_OP_DUP_HPP
#define CIDK_OP_DUP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct DupType: OpType {
    DupType(string id);
    virtual void eval(Cx &cx, const Op &op) const override;
  };

  extern const DupType Dup;
}


namespace cidk {
  template <>
  inline void init(Op &op, const ops::DupType &type) { }
}

#endif
