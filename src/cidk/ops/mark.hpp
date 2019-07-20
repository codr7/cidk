#ifndef CIDK_OP_MARK_HPP
#define CIDK_OP_MARK_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct MarkType: OpType {
    MarkType(const string &id);
    void init(Cx &cx, Op &op) const;
    virtual void eval(Op &op, Env &env, Stack &stack) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const MarkType Mark;
}

#endif
