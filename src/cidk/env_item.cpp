#include "cidk/cx.hpp"
#include "cidk/env_item.hpp"

namespace cidk {
  EnvItem::EnvItem(Env *env, const Val &val): env(env), val(val), nrefs(1) {}

  void EnvItem::deref(Cx &cx) {
    if (!--nrefs) { cx.env_item_pool.put(this); }
  }
}
