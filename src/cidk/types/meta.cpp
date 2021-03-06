#include <iostream>
#include "cidk/sym.hpp"
#include "cidk/types/meta.hpp"
#include "cidk/val.hpp"

namespace cidk {
  MetaType::MetaType(Cx &cx,
                     const Pos &pos,
                     Env &env,
                     const Sym *id,
                     const vector<Type *> &parents):
    TValType<Type *>(cx, pos, env, id, parents) {}

  void MetaType::dump(const Val &val, ostream &out) const {
    out << val.as_type->id << ":Meta";
  }

  bool MetaType::is(const Val &x, const Val &y) const {
    return x.as_type == y.as_type;
  }

  void MetaType::mark_refs(const Val &val) const { val.as_type->mark(); }

  void MetaType::set(Val &dst, Type *val) const { dst.as_type = val; }

  void MetaType::sweep(const Pos &pos, Val &val) { val.as_type->sweep(cx, pos); }
}
