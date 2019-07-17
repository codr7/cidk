#ifndef CIDK_OP_SUB_HPP
#define CIDK_OP_SUB_HPP

#include "cidk/bin_op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct SubType: BinOp {
    SubType(const string &id);
    virtual const Sym *get_fun_id(Cx &cx) const override;
  };

  extern const SubType Sub;
}

#endif
