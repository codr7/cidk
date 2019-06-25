#ifndef CIDK_TYPE_HPP
#define CIDK_TYPE_HPP

#include "cidk/def.hpp"
#include "cidk/env.hpp"
#include "cidk/ops.hpp"

namespace cidk {
  struct Val;
  struct Pos;

  struct Type: Def {
    Env env;
    
    Type(Cx &cx, const Pos &pos, const string &id);

    void call_env(const Pos &pos, const Sym *id);
    virtual void init();
  };
}

#endif
