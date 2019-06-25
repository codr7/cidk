#ifndef CIDK_DEF_HPP
#define CIDK_DEF_HPP

#include <string>

#include "cidk/pos.hpp"

namespace cidk {
  using namespace std;
  
  struct Cx;
  struct Sym;
  
  struct Def {
    Cx &cx;
    Pos pos;
    const Sym *id;
    
    Def(Cx &cx, const Pos &pos, const string &id);
  };
}

#endif
