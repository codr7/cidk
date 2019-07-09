#include "cidk/cx.hpp"
#include "cidk/def.hpp"

namespace cidk {
  Def::Def(Cx &cx, const Pos &pos, const Sym *id):
    Ref(cx), pos(pos), id(id) {}
}
