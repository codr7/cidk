#ifndef CIDK_ENV_ITEM_HPP
#define CIDK_ENV_ITEM_HPP

#include "cidk/ref.hpp"
#include "cidk/val.hpp"

namespace cidk {
  struct Env;

  struct EnvItem: Ref {
    Pos pos;
    Env *env;
    Val val;
    
    EnvItem(const Pos &pos, Env *Env, const Val &val);
    void mark();
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif