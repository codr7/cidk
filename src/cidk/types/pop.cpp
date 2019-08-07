#include <iostream>

#include "cidk/cx.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/val.hpp"

namespace cidk {
  PopType::PopType(Cx &cx,
                   const Pos &pos,
                   const Sym *id,
                   const vector<Type *> &parents):
    ValType(cx, pos, id, parents) {}

  void PopType::cp(Val &dst, const Val &src) const {}

  void PopType::dump(const Val &val, ostream &out) const { out << '$'; }

  void PopType::eval(Cx &cx,
                     const Pos &pos,
                     const Val &val,
                     Env &env,
                     Regs &regs,
                     Stack &stack) const {}

  bool PopType::is(const Val &x, const Val &y) const { return false; }
}
