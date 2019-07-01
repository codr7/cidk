#ifndef CIDK_OP_CALL_HPP
#define CIDK_OP_CALL_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct CallType: OpType {
    CallType(string id);
    void init(Op &op, Fun *fun) const;
    virtual void eval(const Op &op, Env &env) const override;

    virtual void read(Cx &cx,
                      const Pos &pos,
                      Reader &in,
                      Env &env,
                      Ops &out) const override;
  };

  extern const CallType Call;
}

#endif
