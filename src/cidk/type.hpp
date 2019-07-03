#ifndef CIDK_TYPE_HPP
#define CIDK_TYPE_HPP

#include "cidk/def.hpp"
#include "cidk/env.hpp"

namespace cidk {
  struct Val;
  struct Pos;

  struct Type: Def {
    Env &env;
    
    Type(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    void derive(Type &parent);
    void mark(const Pos &pos);
    virtual void sweep(const Pos &pos) override;
  };
}

#endif
