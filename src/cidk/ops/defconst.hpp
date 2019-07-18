#ifndef CIDK_OP_DEFINE_HPP
#define CIDK_OP_DEFINE_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct DefconstType: OpType {
    DefconstType(const string &id);

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      ReadState &state,                     
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const DefconstType Defconst;
}

#endif
