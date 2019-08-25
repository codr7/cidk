#include "cidk/cx.hpp"
#include "cidk/type.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Type::Type(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents):
    Def(cx, pos, id), tag(cx.type_tag++) {
    if (cx.type_tag == CIDK_TYPE_MAX) { throw ESys(pos, "Max types exceeded"); }
    init_parents();
    for (auto pt: parents) { derive(cx, *pt); }
  }

  void Type::init_parents() { fill(parents.begin(), parents.end(), nullptr); }
  
  void Type::derive(Cx &cx, Type &parent) { derive(cx, parent, parent); }
    
  void Type::derive(Cx &cx, Type &parent, Type &root) {
    parents[parent.tag] = &root;

    for(auto pp: parent.parents) {
      if (pp) { derive(cx, *pp, root); }
    }
  }
  
  void Type::mark() { ref_mark = true; }
  
  void Type::sweep(Cx &cx, const Pos &pos) { delete this; }
}
