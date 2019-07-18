#ifndef CIDK_OP_INCLUDE_HPP
#define CIDK_OP_INCLUDE_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct IncludeType: OpType {
    IncludeType(const string &id);

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      ReadState &state,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const IncludeType Include;
}

#endif
