#include <iostream>
#include "cidk/types/meta.hpp"
#include "cidk/val.hpp"

namespace cidk {
  MetaType::MetaType(Cx &cx, const Pos &pos, const Sym *id):
    ValTType<Type *>(cx, pos, id) { }

  void MetaType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << val.as_type->id;
  }

  void MetaType::dup(const Pos &pos, Val &dst, const Val &src) const {
    dst.as_type = src.as_type;
  }

  bool MetaType::is(const Pos &pos, const Val &x, const Val &y) const {
    return x.as_type == y.as_type;
  }

  void MetaType::mark_refs(const Pos &pos, const Val &val) {
    Type &t(*val.as_type);
    t.ref_state = RefState::mark;
    t.env.ref_state = RefState::mark;
  }

  void MetaType::set(const Pos &pos, Val &dst, Type *val) const {
    dst.as_type = val;
  }

  void MetaType::sweep(const Pos &pos, Val &val) {
    val.as_type->sweep(pos);
  }
}
