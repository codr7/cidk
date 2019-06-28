#ifndef CIDK_OP_PUSH_HPP
#define CIDK_OP_PUSH_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct PushType: OpType {
    PushType(string id);
    void init(Op &op, const Val &val) const;
    virtual void eval(Cx &cx, const Op &op) const override;
    virtual void read(Cx &cx, const Pos &pos, Reader &in, Ops &out) const override;
  };

  extern const PushType Push;
}

#endif
