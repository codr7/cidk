#include "cidk/call.hpp"
#include "cidk/cmp.hpp"
#include "cidk/sym.hpp"
#include "cidk/types/lib.hpp"
#include "cidk/val.hpp"

namespace cidk {
  LibType::LibType(Cx &cx,
                   const Pos &pos,
                   Env &env,
                   const Sym *id,
                   const vector<Type *> &parents):
    TValType<Lib *>(cx, pos, env, id, parents) {}

  int LibType::cmp(const Pos &pos, const Val &x, const Val &y) const {
    return cidk::cmp(pos, x.as_lib, y.as_lib);
  }

  void LibType::dump(const Val &val, ostream &out) const {
    out << val.as_lib->id << ":Lib";
  }

  bool LibType::is(const Val &x, const Val &y) const { return x.as_lib == y.as_lib; }

  void LibType::mark_refs(const Val &val) const { val.as_lib->mark(); }

  void LibType::set(Val &dst, Lib *val) const { dst.as_lib = val; }

  void LibType::sweep(const Pos &pos, Val &val) { val.as_lib->sweep(cx, pos); }
}
