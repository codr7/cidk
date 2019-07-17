#ifndef CIDK_OP_EQ_HPP
#define CIDK_OP_EQ_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct EqType: OpType {
    EqType(const string &id);
    void init(Op &op, const Val &x, const Val &y) const;
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

  extern const EqType Eq;
}

#endif