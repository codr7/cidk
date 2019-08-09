#ifndef CIDK_OP_INCLUDE_HPP
#define CIDK_OP_INCLUDE_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct IncludeType: OpType {
    IncludeType(const string &id);

    void init(Cx &cx, Op &op, string val) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const IncludeType Include;
}

#endif
