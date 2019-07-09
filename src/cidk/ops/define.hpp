#ifndef CIDK_OP_DEFINE_HPP
#define CIDK_OP_DEFINE_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct DefineType: OpType {
    DefineType(const string &id);
    void init(Op &op, const Val &val) const;
    virtual void eval(const Op &op, Env &env, Stack &stack) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      ReadState &state,                     
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const DefineType Define;
}

#endif
