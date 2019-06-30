#ifndef CIDK_OP_IF_HPP
#define CIDK_OP_IF_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct IfType: OpType {
    IfType(string id);
    void init(Op &op, const Val &cond, const Val &x, const Val &y) const;
    virtual void eval(const Op &op, Env &env) const override;
    virtual void read(Cx &cx, const Pos &pos, Reader &in, Ops &out) const override;
  };

  extern const IfType If;
}

#endif
