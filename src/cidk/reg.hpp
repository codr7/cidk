#ifndef CIDK_REG_HPP
#define CIDK_REG_HPP

#include <vector>

namespace cidk {
  using namespace std;

  struct Sym;
  struct Val;

  using Regs = vector<pair<const Sym *, Val>>;

  void set_reg(Regs &in, size_t i, const Sym *id, const Val &val);
}

#endif
