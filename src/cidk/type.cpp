#include "cidk/cx.hpp"
#include "cidk/type.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Type::Type(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents):
    Def(cx, pos, id), env(*cx.env_pool.get(cx)) {
    for (auto pt: parents) { derive(cx, *pt); }
  }
  
  void Type::derive(Cx &cx, Type &parent) {
    env.merge(cx, parent.env);
    Type *p(&parent);
    parents.emplace(p, p);
    
    for(auto &i: parent.parents) {
      Type *pp(i.first);
      env.merge(cx, pp->env);
      parents.emplace(pp, p);
    }
  }

  Type *Type::isa(Type &parent) const {
    Type *p(&parent);
    if (p == this) { return p; }
    auto i(parents.find(p));
    return (i == parents.end()) ? nullptr : i->second;
  }
  
  void Type::mark() {
    if (!ref_mark) {
      ref_mark = true;
      env.mark();
    }
  }
  
  void Type::sweep(Cx &cx, const Pos &pos) { delete this; }
}
