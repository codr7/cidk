#ifndef CIDK_OP_DO_STACK_HPP
#define CIDK_OP_DO_STACK_HPP

#include "cidk/op.hpp"

namespace cidk::ops {
  struct DoStackType: OpType {
    DoStackType(string id);
    void init(Op &op, const Val &in, const Val &body) const;
    virtual void eval(const Op &op, Env &env, Stack &stack) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const DoStackType DoStack;
}

#endif
