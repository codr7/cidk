#include "cidk/cx.hpp"
#include "cidk/type.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Type::Type(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents):
    Def(cx, pos, id), env(*cx.env_pool.get(cx)) {
    for (auto pt: parents) { derive(*pt); }
  }
  
  void Type::derive(Type &parent) { env.merge(parent.env); }
  
  void Type::mark() {
    if (!ref_mark) {
      ref_mark = true;
      env.mark();
    }
  }
  
  void Type::sweep(Cx &cx, const Pos &pos) { delete this; }
}
