#ifndef CIDK_OP_ADD_HPP
#define CIDK_OP_ADD_HPP

#include "cidk/bin_op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct AddType: BinOp {
    AddType(const string &id);
    virtual const Sym *get_fun_id(Cx &cx) const override;
  };

  extern const AddType Add;
}

#endif
