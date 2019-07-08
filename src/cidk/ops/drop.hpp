#ifndef CIDK_OP_DROP_HPP
#define CIDK_OP_DROP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct DropType: OpType {
    DropType(const string &id);
    void init(Op &op) const;
    virtual void eval(const Op &op, Env &env, Stack &stack) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const DropType Drop;
}

#endif
