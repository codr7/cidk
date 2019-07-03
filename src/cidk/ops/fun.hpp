#ifndef CIDK_OP_FUN_HPP
#define CIDK_OP_FUN_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct FunType: OpType {
    FunType(string id);
    void init(Op &op, Fun *fun) const;
    virtual void eval(const Op &op, Env &env) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Ops &out) const override;
  };

  extern const FunType Fun;
}

#endif
