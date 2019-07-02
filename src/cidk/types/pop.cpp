#include <iostream>

#include "cidk/cx.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/val.hpp"

namespace cidk {
  PopType::PopType(Cx &cx, const Pos &pos, const Sym *id):
    ValType(cx, pos, id) { }

  void PopType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << '$';
  }

  void PopType::dup(Val &dst, const Val &src) const { }

  void PopType::eval(const Pos &pos, const Val &val, Env &env) const {
    pop(pos, env.cx.stack, false)->eval(pos, env);
  }

  bool PopType::is(const Pos &pos, const Val &x, const Val &y) const { return true; }
}
