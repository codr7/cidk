#ifndef CIDK_OP_DO_STACK_HPP
#define CIDK_OP_DO_STACK_HPP

#include "cidk/op.hpp"

namespace cidk::ops {
  struct DoStackType: OpType {
    DoStackType(const string &id);
    void init(Op &op, const Val &in, const Val &body) const;
    virtual void eval(const Op &op, Env &env, Stack &stack) const override;
    virtual void get_ids(const Op &op, IdSet &out) const override;
    virtual void mark_refs(Op &op) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      ReadState &state,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const DoStackType DoStack;
}

#endif
