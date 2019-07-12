#ifndef CIDK_ENV_ITEM_HPP
#define CIDK_ENV_ITEM_HPP

#include "cidk/val.hpp"

namespace cidk {
  struct Env;

  struct EnvItem {
    Env &env;
    Val val;
    int nrefs;
    
    EnvItem(Env &Env, const Val &val);
    void deref(Cx &cx);
  };
}

#endif
