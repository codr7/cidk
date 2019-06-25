#include "cidk/cx.hpp"
#include "cidk/def.hpp"

namespace cidk {
  Def::Def(Cx &cx, const Pos &pos, const string &id):
    cx(cx), pos(pos), id(cx.intern(id)) { }
}
