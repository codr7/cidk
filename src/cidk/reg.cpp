#include "cidk/reg.hpp"
#include "cidk/val.hpp"

namespace cidk {
  void set_reg(Regs &regs, size_t i, const Sym *id, const Val &val) {
    auto rs(regs.size());
    
    if (i == rs) {
      regs.emplace_back(id, val);
    } else {
      if (rs < i + 1) { regs.resize(i + 1); }
      auto &r(regs[i]);
      r.first = id;
      val.cp(r.second);
    }
  }
}
