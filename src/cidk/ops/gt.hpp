#ifndef CIDK_OP_GT_HPP
#define CIDK_OP_GT_HPP

#include "cidk/bin_op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct GtType: BinOp {
    GtType(const string &id);
    virtual const Sym *get_fun_id(Cx &cx) const override;
  };

  extern const GtType Gt;
}

#endif
