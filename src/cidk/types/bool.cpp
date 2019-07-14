#include <iostream>
#include "cidk/types/bool.hpp"
#include "cidk/val.hpp"

namespace cidk {
  BoolType::BoolType(Cx &cx,
                     const Pos &pos,
                     const Sym *id,
                     const vector<Type *> &parents):
    TValType<bool>(cx, pos, id, parents) {}

  void BoolType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << (val.as_bool ? 'T' : 'F');
  }

  void BoolType::dup(Val &dst, const Val &src) const { dst.as_bool = src.as_bool; }

  bool BoolType::is(const Val &x, const Val &y) const {
    return x.as_bool == y.as_bool;
  }

  void BoolType::set(const Pos &pos, Val &dst, bool val) const { dst.as_bool = val; }

  bool BoolType::get_bool(const Val &val) const { return val.as_bool; }
}
