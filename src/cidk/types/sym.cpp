#include <iostream>

#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/types/sym.hpp"
#include "cidk/val.hpp"

namespace cidk {
  SymType::SymType(Cx &cx, const Pos &pos, const Sym *id):
    TValType<const Sym *>(cx, pos, id) { }

  void SymType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << val.as_sym;
  }

  void SymType::dup(Val &dst, const Val &src) const {
    dst.as_sym = src.as_sym;
  }

  void SymType::eval(const Pos &pos, const Val &val, Env &env) const {
    Val out;
    env.get(pos, val.as_sym, out, false);
    cx.stack.push_back(out);
  }
    
  bool SymType::is(const Pos &pos, const Val &x, const Val &y) const {
    return x.as_sym == y.as_sym;
  }

  void SymType::set(const Pos &pos, Val &dst, const Sym *val) const {
    dst.as_sym = val;
  }
}
