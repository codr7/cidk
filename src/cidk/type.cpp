#include "cidk/cx.hpp"
#include "cidk/type.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Type::Type(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents):
    Def(cx, pos, id) {
    for (auto pt: parents) { derive(cx, *pt); }
  }
  
  void Type::derive(Cx &cx, Type &parent) {
    Type *p(&parent);
    parents.emplace(p, p);
    
    for(auto &i: parent.parents) {
      Type *pp(i.first);
      parents.emplace(pp, p);
    }
  }

  Type *Type::isa(Type &parent) const {
    Type *p(&parent);
    if (p == this) { return p; }
    auto i(parents.find(p));
    return (i == parents.end()) ? nullptr : i->second;
  }
  
  void Type::mark() { ref_mark = true; }
  
  void Type::sweep(Cx &cx, const Pos &pos) { delete this; }
}
