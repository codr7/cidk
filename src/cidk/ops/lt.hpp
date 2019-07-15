#ifndef CIDK_OP_LT_HPP
#define CIDK_OP_LT_HPP

#include "cidk/bin_op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct LtType: BinOp {
    LtType(const string &id);
    virtual const Sym *get_fun_id(Cx &cx) const override;
  };

  extern const LtType Lt;
}

#endif
