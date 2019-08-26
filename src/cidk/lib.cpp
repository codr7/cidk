#include "cidk/lib.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Lib::Lib(Cx &cx, const Pos &pos, const Sym *id): Def(cx, pos, id), env(cx) {}

  void Lib::mark() {
    if (!ref_mark) { env.mark_refs(); }
  }
    
  void Lib::sweep(Cx &cx, const Pos &pos) { delete this; }
}
