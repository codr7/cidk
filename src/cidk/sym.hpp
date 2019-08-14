#ifndef CIDK_SYM_HPP
#define CIDK_SYM_HPP

#include "cidk/def.hpp"

namespace cidk {
  struct Type;
  
  struct Sym {
    string name;
    const Sym *root;
    
    Sym(string name);
  };

  ostream &operator <<(ostream &out, const Sym *sym);
}

#endif
