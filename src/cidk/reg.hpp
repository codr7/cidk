#ifndef CIDK_REG_HPP
#define CIDK_REG_HPP

#include <vector>

namespace cidk {
  using namespace std;

  struct Sym;
  struct Val;

  using Reg = Val;

  void set_reg(Reg *base, size_t n, const Sym *id, const Val &src);
}

#endif
