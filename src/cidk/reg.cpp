#include "cidk/reg.hpp"
#include "cidk/val.hpp"

namespace cidk {
  void set_reg(Reg *regs, size_t i, const Sym *id, const Val &val) {
    auto &r(regs[i]);
    r.first = id;
    val.cp(r.second);
  }
}
