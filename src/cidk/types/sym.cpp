#include <iostream>

#include "cidk/cx.hpp"
#include "cidk/types/env.hpp"
#include "cidk/types/sym.hpp"
#include "cidk/val.hpp"

namespace cidk {
  SymType::SymType(Cx &cx,
                   const Pos &pos,
                   const Sym *id,
                   const vector<Type *> &parents):
    TValType<const Sym *>(cx, pos, id, parents) {}

  void SymType::compile(Cx &cx,
                        const Pos &pos,
                        Val &val,
                        Env &env,
                        Stack &stack,
                        Opts *opts) const {
    if (auto i(env.try_get(val.as_sym)); i) {
      i->val.eval(cx, pos, env, stack);
      val = pop(pos, stack);
    }
  }

  void SymType::cp(Val &dst, const Val &src) const { dst.as_sym = src.as_sym; }

  void SymType::dump(const Val &val, ostream &out) const { out << val.as_sym; }

  void SymType::eval(Cx &cx,
                     const Pos &pos,
                     const Val &val,
                     Env &env,
                     Stack &stack) const {
    env.get(pos, val.as_sym).eval(cx, pos, env, stack);
  }

  void SymType::get_ids(const Val &val, IdSet &out) const { out.emplace(val.as_sym); }

  bool SymType::is(const Val &x, const Val &y) const { return x.as_sym == y.as_sym; }

  void SymType::set(Val &dst, const Sym *val) const { dst.as_sym = val; }
}
