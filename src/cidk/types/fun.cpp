#include <iostream>

#include "cidk/call.hpp"
#include "cidk/cmp.hpp"
#include "cidk/sym.hpp"
#include "cidk/types/fun.hpp"
#include "cidk/val.hpp"

namespace cidk {
  FunType::FunType(Cx &cx,
                   const Pos &pos,
                   const Sym *id,
                   const vector<Type *> &parents):
    TValType<Fun *>(cx, pos, id, parents) {}

  int FunType::cmp(const Pos &pos, const Val &x, const Val &y) const {
    return cidk::cmp(pos, x.as_fun, y.as_fun);
  }

  void FunType::cp(Val &dst, const Val &src) const { dst.as_fun = src.as_fun; }

  void FunType::dump(const Val &val, ostream &out) const {
    out << val.as_fun->id << ":Fun";
  }

  bool FunType::is(const Val &x, const Val &y) const { return x.as_fun == y.as_fun; }

  void FunType::mark_refs(const Val &val) const { val.as_fun->mark(); }

  void FunType::set(Val &dst, Fun *val) const { dst.as_fun = val; }

  void FunType::sweep(const Pos &pos, Val &val) { val.as_fun->sweep(cx, pos); }
}
