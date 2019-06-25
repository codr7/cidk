#ifndef CIDK_TYPE_ANY_HPP
#define CIDK_TYPE_ANY_HPP

#include "cidk/type.hpp"

namespace cidk {  
  struct AnyType: Type {
    AnyType(Cx &cx, const Pos &pos, string id);
  };
}

#endif
