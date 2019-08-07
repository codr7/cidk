#ifndef CIDK_EXT_ID_HPP
#define CIDK_EXT_ID_HPP

#include "cidk/val.hpp"

namespace cidk {
  struct ExtId {
    const Sym *id;
    size_t src_reg, dst_reg;
    Val val;
    
    ExtId(const Sym *id, size_t dst_reg);
  };
}

#endif
