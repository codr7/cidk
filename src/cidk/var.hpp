#ifndef CIDK_VAR_HPP
#define CIDK_VAR_HPP

#include "cidk/val.hpp"

namespace cidk {
  struct Env;
  
  struct Var {
    Pos pos;
    Env *env;
    Val val;

    Var(const Pos &pos, Env *Env, const Val &val);
  };
}

#endif
