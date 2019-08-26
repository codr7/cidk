#include "cidk/types/funs.hpp"

namespace cidk {
  FunsType::FunsType(Cx &cx,
                     const Pos &pos,
                     const Sym *id,
                     const vector<Type *> &parents):
    ListType(cx, pos, id, parents) {}
}
