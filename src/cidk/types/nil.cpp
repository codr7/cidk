#include <iostream>
#include "cidk/types/nil.hpp"
#include "cidk/val.hpp"

namespace cidk {
  NilType::NilType(Cx &cx,
                   const Pos &pos,
                   const Sym *id,
                   const vector<Type *> &parents):
    ValType(cx, pos, id, parents) {}

  void NilType::cp(Val &dst, const Val &src) const {}

  void NilType::dump(const Val &val, ostream &out) const { out << '_'; }

  bool NilType::is(const Val &x, const Val &y) const { return true; }

  bool NilType::get_bool(const Val &val) const { return false; }
}
