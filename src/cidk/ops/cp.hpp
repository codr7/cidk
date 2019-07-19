#ifndef CIDK_OP_CP_HPP
#define CIDK_OP_CP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct CpType: OpType {
    CpType(const string &id);
    void init(Cx &cx, Op &op, Int offs, Int len) const;
    virtual void eval(Op &op, Env &env, Stack &stack) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      ReadState &state,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const CpType Cp;
}

#endif