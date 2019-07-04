#ifndef CIDK_OP_CONST_HPP
#define CIDK_OP_CONST_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct ConstType: OpType {
    ConstType(string id);
    void init(Op &op, const Val &val) const;
    virtual void eval(const Op &op, Env &env, Stack &stack) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const ConstType Const;
}

#endif
