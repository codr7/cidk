#include "cidk/cx.hpp"
#include "cidk/var.hpp"

namespace cidk {
  Var::Var(const Pos &pos, Env *env, const Val &val):
    Ref(env->cx), cx(env->cx), pos(pos), env(env), val(val) { }

  void Var::mark(const Pos &pos) {
    ref_state = RefState::mark;
    val.mark_refs(pos);
  }
  
  void Var::sweep(const Pos &pos) { cx.var_pool.put(this); }
}
