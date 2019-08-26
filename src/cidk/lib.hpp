#ifndef CIDK_LIB_HPP
#define CIDK_LIB_HPP

#include "cidk/def.hpp"
#include "cidk/env.hpp"

namespace cidk {
  struct Lib: Def {
    Env env;

    Lib(Cx &cx, const Pos &pos, const Sym *id);

    void mark();
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
