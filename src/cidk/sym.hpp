#ifndef CIDK_SYM_HPP
#define CIDK_SYM_HPP

#include "cidk/def.hpp"

namespace cidk {
  struct Sym {
    string name;
    
    Sym(string name);
  };

  ostream &operator <<(ostream &out, const Sym *sym);
}

#endif
