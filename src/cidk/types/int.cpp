#include "cidk/cx.hpp"
#include "cidk/types/int.hpp"
#include "cidk/val.hpp"

namespace cidk {
  IntType::IntType(Cx &cx,
                   const Pos &pos,
                   const Sym *id,
                   const vector<Type *> &parents):
    TValType<Int>(cx, pos, id, parents) {}

  int IntType::cmp(const Pos &pos, const Val &x, const Val &y) const {
    return cidk::cmp(pos, x.as_int, y.as_int);
  }

  void IntType::dump(const Val &val, ostream &out) const { out << val.as_int; }

  bool IntType::is(const Val &x, const Val &y) const { return x.as_int == y.as_int; }

  void IntType::set(Val &dst, Int val) const { dst.as_int = val; }

  bool IntType::get_bool(const Val &val) const { return val.as_int > 0; }
}
