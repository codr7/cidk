#ifndef CIDK_OP_STACK_HPP
#define CIDK_OP_STACK_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct StackType: OpType {
    StackType(const string &id);
    void init(Op &op) const;
    
    virtual void eval(const Op &op,
                      cidk::Env &env,
                      cidk::Stack &stack) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      ReadState &state,
                      cidk::Env &env,
                      cidk::Stack &stack,
                      Ops &out) const override;
  };

  extern const StackType Stack;
}

#endif
