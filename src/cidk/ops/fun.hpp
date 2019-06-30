#ifndef CIDK_OP_FUN_HPP
#define CIDK_OP_FUN_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct FunType: OpType {
    FunType(string id);
    void init(Op &op, Fun *fun) const;
    virtual void eval(const Op &op, Env &env) const override;
    virtual void read(Cx &cx, const Pos &pos, Reader &in, Ops &out) const override;
  };

  extern const FunType Fun;
}

#endif
