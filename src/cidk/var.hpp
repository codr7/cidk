#ifndef CIDK_VAR_HPP
#define CIDK_VAR_HPP

#include "cidk/ref.hpp"
#include "cidk/val.hpp"

namespace cidk {
  struct Env;
  
  struct Var: Ref {
    Pos pos;
    Env *env;
    Val val;

    Var(const Pos &pos, Env *Env, const Val &val);
    void mark();
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
