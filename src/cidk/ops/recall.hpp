#ifndef CIDK_OP_RECALL_HPP
#define CIDK_OP_RECALL_HPP

#include "cidk/cx.hpp"
#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct RecallType: OpType {
    RecallType(const string &id);
    void init(Cx &cx, Op &op) const;

    virtual void eval(Cx &cx, Op &op, Env &env, Reg *regs) const override {
      cx.eval_state = EvalState::recall;
    }

    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const RecallType Recall;
}

#endif
