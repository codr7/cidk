#include <iostream>
#include "cidk/types/nil.hpp"
#include "cidk/val.hpp"

namespace cidk {
  NilType::NilType(Cx &cx,
                   const Pos &pos,
                   const Sym *id,
                   const vector<Type *> &parents):
    ValType(cx, pos, id, parents) { }

  void NilType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << '_';
  }

  void NilType::dup(Val &dst, const Val &src) const { }

  bool NilType::is(const Pos &pos, const Val &x, const Val &y) const { return true; }

  bool NilType::Bool(const Pos &pos, const Val &val) const { return false; }
}
