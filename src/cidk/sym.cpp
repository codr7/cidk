#include "cidk/sym.hpp"

namespace cidk {
  Sym::Sym(string name): name(name) {}

  ostream &operator <<(ostream &out, const Sym *sym) {
    out << sym->name;
    return out;
  }
}
