#ifndef CIDK_OP_PUSH_HPP
#define CIDK_OP_PUSH_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct PushType: OpType {
    PushType(string id);
    virtual void eval(Cx &cx, const Op &op) const override;
  };

  extern const PushType Push;
  
  void init(Op &op, const ops::PushType &type, const Val &val);
}

#endif
