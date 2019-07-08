#ifndef CIDK_OP_IS_HPP
#define CIDK_OP_IS_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct IsType: OpType {
    IsType(const string &id);
    void init(Op &op, const Val &x, const Val &y) const;
    virtual void eval(const Op &op, Env &env, Stack &stack) const override;
    virtual void get_ids(const Op &op, IdSet &out) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const IsType Is;
}

#endif
