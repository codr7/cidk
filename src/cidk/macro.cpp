#include "cidk/cx.hpp"
#include "cidk/macro.hpp"
#include "cidk/types/macro.hpp"

namespace cidk {
  Macro::Macro(Cx &cx,
               const Pos &pos,
               const Sym *id,
               Imp imp): Def(cx, pos, id), imp(imp) {}

  void Macro::call(Pos &pos, istream &in, Env &env, Stack &stack, Ops &out) const {
    imp(pos, *this, in, env, stack, out);
  }

  void Macro::mark() {
    if (ref_state == RefState::sweep) { ref_state = RefState::keep; }
  }
  
  void Macro::sweep(Cx &cx, const Pos &pos) { cx.macro_type.pool.put(this); }
}
