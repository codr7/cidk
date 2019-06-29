#include <iostream>

#include "cidk/call.hpp"
#include "cidk/types/sym.hpp"
#include "cidk/val.hpp"

namespace cidk {
  SymType::SymType(Cx &cx, const Pos &pos, string id):
    ValTType<const Sym *>(cx, pos, id) { }

  void SymType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << val.as_sym;
  }

  void SymType::dup(const Pos &pos, Val &dst, const Val &src) const {
    dst.as_sym = src.as_sym;
  }

  bool SymType::is(const Pos &pos, const Val &x, const Val &y) const {
    return x.as_sym == y.as_sym;
  }

  void SymType::set(const Pos &pos, Val &dst, const Sym *val) const {
    dst.as_sym = val;
  }
}
