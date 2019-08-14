#include "cidk/cx.hpp"
#include "cidk/type.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Type::Type(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents):
    Def(cx, pos, id), tag(cx.type_tag++) {
    if (cx.type_tag == CIDK_TYPE_MAX) { throw ESys(pos, "Max types exceeded"); }
    for (auto pt: parents) { derive(cx, *pt); }
  }
  
  void Type::derive(Cx &cx, Type &parent) {
    parents[parent.tag] = &parent;

    for(auto pp: parent.parents) {
      if (pp) { derive(cx, *pp); }
    }
  }

  Type *Type::isa(Type &parent) const {
    Type *p(&parent);
    if (p == this) { return p; }
    return parents[parent.tag];
  }
  
  void Type::mark() { ref_mark = true; }
  
  void Type::sweep(Cx &cx, const Pos &pos) { delete this; }
}
