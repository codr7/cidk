#ifndef CIDK_OP_MUL_HPP
#define CIDK_OP_MUL_HPP

#include "cidk/bin_op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct MulType: BinOp {
    MulType(const string &id);
    virtual const Sym *get_fun_id(Cx &cx) const override;
  };

  extern const MulType Mul;
}

#endif
