#ifndef CIDK_OP_CALL_HPP
#define CIDK_OP_CALL_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct CallType: OpType {
    CallType(string id);
    virtual void eval(Cx &cx, const Op &op) const override;
  };

  extern const CallType Call;

  void init(Op &op, const ops::CallType &type, Fun *fun = nullptr);
}

#endif
