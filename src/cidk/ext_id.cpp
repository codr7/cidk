#include "cidk/ext_id.hpp"

namespace cidk {
  ExtId::ExtId(const Pos &pos, const Sym *id, size_t dst_reg):
    pos(pos), id(id), dst_reg(dst_reg) {}

  void ExtId::mark_refs() {
    if (val.type) { val.mark_refs(); }
  }
}
