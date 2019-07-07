#include <iostream>
#include "cidk/sym.hpp"
#include "cidk/types/meta.hpp"
#include "cidk/val.hpp"

namespace cidk {
  MetaType::MetaType(Cx &cx,
                     const Pos &pos,
                     const Sym *id,
                     const vector<Type *> &parents):
    TValType<Type *>(cx, pos, id, parents) { }

  void MetaType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << "type:" << val.as_type->id;
  }

  void MetaType::dup(Val &dst, const Val &src) const {
    dst.as_type = src.as_type;
  }

  bool MetaType::is(const Val &x, const Val &y) const {
    return x.as_type == y.as_type;
  }

  void MetaType::mark_refs(const Pos &pos, const Val &val) const {
    val.as_type->mark(pos);
  }

  void MetaType::set(const Pos &pos, Val &dst, Type *val) const {
    dst.as_type = val;
  }

  void MetaType::sweep(const Pos &pos, Val &val) {
    val.as_type->sweep(pos);
  }
}
