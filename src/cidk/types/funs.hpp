#ifndef CIDK_TYPE_FUNS_HPP
#define CIDK_TYPE_FUNS_HPP

#include "cidk/types/list.hpp"

namespace cidk {
  struct FunsType: ListType {
    FunsType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);
  };
}

#endif
