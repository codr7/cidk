#include <iostream>

#include "cidk/call.hpp"
#include "cidk/types/fun.hpp"
#include "cidk/val.hpp"

namespace cidk {
  FunType::FunType(Cx &cx, const Pos &pos, const Sym *id):
    ValTType<Fun *>(cx, pos, id) { }

  void FunType::call(const Pos &pos, const Val &val) const {
    Call(cx, pos, *val.as_fun).eval();
  }

  void FunType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << val.as_fun->id << "()()";
  }

  void FunType::dup(const Pos &pos, Val &dst, const Val &src) const {
    dst.as_fun = src.as_fun;
  }

  bool FunType::is(const Pos &pos, const Val &x, const Val &y) const {
    return x.as_fun == y.as_fun;
  }

  void FunType::mark_refs(const Pos &pos, const Val &val) {
    Fun &f(*val.as_fun);
    f.ref_state = RefState::mark;
    f.env.ref_state = RefState::mark;
  }

  void FunType::set(const Pos &pos, Val &dst, Fun *val) const {
    dst.as_fun = val;
  }

  void FunType::sweep(const Pos &pos, Val &val) {
    val.as_fun->sweep(pos);
  }
}
