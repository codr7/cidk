#ifndef CIDK_DEF_HPP
#define CIDK_DEF_HPP

#include <string>

#include "cidk/pos.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  using namespace std;
  
  struct Sym;
  
  struct Def: Ref {
    Pos pos;
    const Sym *id;
    
    Def(Cx &cx, const Pos &pos, const Sym *id);
  };
}

#endif
