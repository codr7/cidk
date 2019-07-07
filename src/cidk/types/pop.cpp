#include <iostream>

#include "cidk/cx.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/val.hpp"

namespace cidk {
  PopType::PopType(Cx &cx,
                   const Pos &pos,
                   const Sym *id,
                   const vector<Type *> &parents):
    ValType(cx, pos, id, parents) { }

  void PopType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << '$';
  }

  void PopType::dup(Val &dst, const Val &src) const { }

  void PopType::eval(const Pos &pos, const Val &val, Env &env, Stack &stack) const {
    pop(pos, stack, false)->eval(pos, env, stack);
  }

  bool PopType::is(const Val &x, const Val &y) const { return true; }
}
