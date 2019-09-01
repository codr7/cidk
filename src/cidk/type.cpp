#include "cidk/cx.hpp"
#include "cidk/type.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Type::Type(Cx &cx,
             const Pos &pos,
             Env &env,
             const Sym *id,
             const vector<Type *> &parents):
    Def(cx, pos, id), env(env), tag(cx.type_tag++), nil_type(nullptr) {
    if (cx.type_tag == CIDK_TYPE_MAX) { throw ESys(pos, "Max types exceeded"); }
    init_parents();
    for (auto pt: parents) { derive(*pt); }
  }

  void Type::init_parents() { fill(parents.begin(), parents.end(), nullptr); }
  
  void Type::derive(Type &parent) { derive(parent, parent); }
    
  void Type::derive(Type &parent, Type &root) {
    Type *&dst(parents[parent.tag]);
    if (!dst) { dst = &root; }

    for(auto pp: parent.parents) {
      if (pp) { derive(*pp, root); }
    }
  }
  
  void Type::mark() { 
    ref_mark = true; 
    if (nil_type && nil_type != this) { nil_type->mark(); }
  }

  Type &Type::or_nil(const Pos &pos) {
    if (!nil_type) {
      nil_type = &env.add_type<Type>(cx, pos, str(id->name, '?'), {&cx.a_type});
      derive(*nil_type);
      cx.nil_type.derive(*nil_type);
    }

    return *nil_type;
  }

  void Type::sweep(Cx &cx, const Pos &pos) { delete this; }
}
