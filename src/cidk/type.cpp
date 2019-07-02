#include "cidk/cx.hpp"
#include "cidk/type.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Type::Type(Cx &cx, const Pos &pos, const Sym *id):
    Def(cx, pos, id), env(*cx.env_pool.get(cx)) {
    cx.types.push_back(this);
  }

  void Type::init() { }

  void Type::mark(const Pos &pos) {
    ref_state = RefState::mark;
    env.ref_state = RefState::mark;
    env.mark_refs(pos);
  }
  
  void Type::sweep(const Pos &pos) { delete this; }
}
