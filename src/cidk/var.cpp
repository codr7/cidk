#include "cidk/cx.hpp"
#include "cidk/var.hpp"

namespace cidk {
  Var::Var(const Pos &pos, Env *env, const Val &val):
    Ref(env->cx), pos(pos), env(env), val(val) { }

  void Var::mark() {
    is_marked = true;
    val.mark_refs();
  }
  
  void Var::sweep(const Pos &pos) { cx.var_pool.put(this); }
}
