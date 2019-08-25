#include <iostream>
#include "cidk/types/bool.hpp"
#include "cidk/val.hpp"

namespace cidk {
  BoolType::BoolType(Cx &cx,
                     const Pos &pos,
                     const Sym *id,
                     const vector<Type *> &parents):
    TValType<bool>(cx, pos, id, parents) {}

  void BoolType::dump(const Val &val, ostream &out) const {
    out << (val.as_bool ? 'T' : 'F');
  }

  bool BoolType::is(const Val &x, const Val &y) const {
    return x.as_bool == y.as_bool;
  }

  void BoolType::set(Val &dst, bool val) const { dst.as_bool = val; }

  bool BoolType::get_bool(const Val &val) const { return val.as_bool; }
}
