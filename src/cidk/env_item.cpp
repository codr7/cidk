#include "cidk/cx.hpp"
#include "cidk/env_item.hpp"

namespace cidk {
  EnvItem::EnvItem(const Pos &pos, Env *env, const Sym *key, const Val &val):
    Ref(env->cx), pos(pos), env(env), key(key), val(val) {}

  void EnvItem::mark() {
    is_marked = true;
    val.mark_refs();
  }
  
  void EnvItem::sweep(Cx &cx, const Pos &pos) { cx.env_item_pool.put(this); }
}
