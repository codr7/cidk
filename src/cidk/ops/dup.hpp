#ifndef CIDK_OP_DUP_HPP
#define CIDK_OP_DUP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct DupType: OpType {
    DupType(string id);
    void init(Op &op) const;
    virtual void eval(const Op &op, Env &env) const override;

    virtual void read(Cx &cx,
                      const Pos &pos,
                      Reader &in,
                      Env &env,
                      Ops &out) const override;
  };

  extern const DupType Dup;
}

#endif
