#ifndef CIDK_SYM_HPP
#define CIDK_SYM_HPP

#include <unordered_set>

#include "cidk/def.hpp"

namespace cidk {
  using IdSet = unordered_set<const Sym *>;

  struct Sym {
    string name;
    
    Sym(string name);
  };

  ostream &operator <<(ostream &out, const Sym *sym);
}

#endif
