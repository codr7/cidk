#ifndef CIDK_OP_DIV_HPP
#define CIDK_OP_DIV_HPP

#include "cidk/bin_op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct DivType: BinOp {
    DivType(const string &id);
    virtual const Sym *get_fun_id(Cx &cx) const override;
  };

  extern const DivType Div;
}

#endif
