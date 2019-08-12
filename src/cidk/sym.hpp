#ifndef CIDK_SYM_HPP
#define CIDK_SYM_HPP

#include "cidk/def.hpp"

namespace cidk {
  struct Type;
  
  struct Sym {
    string name;
    vector<Type *> args;
    
    Sym(string name);
  };

  ostream &operator <<(ostream &out, const Sym *sym);
}

#endif
