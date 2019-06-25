#include "cidk/cx.hpp"
#include "cidk/types/int.hpp"
#include "cidk/val.hpp"

namespace cidk {
  IntType::IntType(Cx &cx, const Pos &pos, const string &id):
    ValTType<Int>(cx, pos, id) { }

  void IntType::dup(const Pos &pos, Val &dst, const Val &src) const {
    dst.as_int = src.as_int;
  }

  bool IntType::is(const Pos &pos, const Val &x, const Val &y) const {
    return x.as_int == y.as_int;
  }

  void IntType::set(const Pos &pos, Val &dst, Int val) const {
    dst.as_int = val;
  }

  void IntType::splat(const Pos &pos, Val &val) {
    auto &s(cx.stack);
    
    for (Int i = 0; i < val.as_int; i++) {
      s.emplace_back(pos, *this, i);
    }
  }
}
