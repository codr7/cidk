#ifndef CIDK_OP_INCLUDE_HPP
#define CIDK_OP_INCLUDE_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct IncludeType: OpType {
    IncludeType(const string &id);

    void init(Cx &cx, Op &op, const Val &fname) const;

    virtual void compile(Cx &cx,
                         Op &op,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const IncludeType Include;
}

#endif
