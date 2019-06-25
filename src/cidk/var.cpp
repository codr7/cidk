#include "cidk/var.hpp"

namespace cidk {
  Var::Var(const Pos &pos, Env *env, const Val &val): pos(pos), env(env), val(val) { }
}
