#include "cidk/ext_id.hpp"

namespace cidk {
  ExtId::ExtId(const Sym *id, size_t dst_reg): id(id), dst_reg(dst_reg) {}
}
