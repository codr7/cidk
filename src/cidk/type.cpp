#include "cidk/cx.hpp"
#include "cidk/type.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Type::Type(Cx &cx, const Pos &pos, const string &id): Def(cx, pos, id), env(cx) {
    cx.types.push_back(this);
  }

  void Type::init() { }

  void Type::call_env(const Pos &pos, const Sym *id) {
    Val target;
    env.get(pos, id, target, false);
    target.call(pos);
  }

  void Type::sweep(const Pos &pos) {
    delete this;
  }
}
