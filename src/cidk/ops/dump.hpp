#ifndef CIDK_OP_DUMP_HPP
#define CIDK_OP_DUMP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct DumpType: OpType {
    DumpType(string id);
    void init(Op &op, const optional<Val> &val) const;
    virtual void eval(Cx &cx, const Op &op) const override;
    virtual void read(Cx &cx, const Pos &pos, Reader &in, Ops &out) const override;
  };

  extern const DumpType Dump;
}

#endif
