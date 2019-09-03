#include "cidk/cmp.hpp"
#include "cidk/sym.hpp"
#include "cidk/types/e.hpp"
#include "cidk/val.hpp"

namespace cidk {
  EType::EType(Cx &cx,
               const Pos &pos,
               Env &env,
               const Sym *id,
               const vector<Type *> &parents):
    TValType<EUser *>(cx, pos, env, id, parents) {}

  int EType::cmp(const Pos &pos, const Val &x, const Val &y) const {
    return cidk::cmp(pos, x.as_e, y.as_e);
  }

  void EType::dump(const Val &val, ostream &out) const {
    out << val.as_e << ":E";
  }

  bool EType::is(const Val &x, const Val &y) const { return x.as_e == y.as_e; }

  void EType::mark_refs(const Val &val) const { val.as_e->mark_refs(); }

  void EType::set(Val &dst, EUser *val) const { dst.as_e = val; }

  void EType::sweep(const Pos &pos, Val &val) { val.as_e->sweep(cx, pos); }
}
