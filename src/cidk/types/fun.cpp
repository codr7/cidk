#include <iostream>

#include "cidk/call.hpp"
#include "cidk/sym.hpp"
#include "cidk/types/fun.hpp"
#include "cidk/val.hpp"

namespace cidk {
  FunType::FunType(Cx &cx,
                   const Pos &pos,
                   const Sym *id,
                   const vector<Type *> &parents):
    TValType<Fun *>(cx, pos, id, parents) { }

  void FunType::call(const Pos &pos, const Val &val, Env &env, Stack &stack) const {
    Call(cx, pos, *val.as_fun).eval(env, stack);
  }

  void FunType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << "fun:" << val.as_fun->id;
  }

  void FunType::dup(Val &dst, const Val &src) const {
    dst.as_fun = src.as_fun;
  }

  bool FunType::is(const Val &x, const Val &y) const {
    return x.as_fun == y.as_fun;
  }

  void FunType::mark_refs(const Pos &pos, const Val &val) const {
    val.as_fun->mark(pos);
  }

  void FunType::set(const Pos &pos, Val &dst, Fun *val) const {
    dst.as_fun = val;
  }

  void FunType::sweep(const Pos &pos, Val &val) {
    val.as_fun->sweep(pos);
  }
}
