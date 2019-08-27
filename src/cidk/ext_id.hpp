#ifndef CIDK_EXT_ID_HPP
#define CIDK_EXT_ID_HPP

#include "cidk/val.hpp"

namespace cidk {
  struct ExtId {
    Pos pos;
    const Sym *id;
    optional<size_t> src_reg;
    size_t dst_reg;
    Val val;
    
    ExtId(const Pos &pos, const Sym *id, size_t dst_reg);
    void mark_refs();
  };
}

#endif
