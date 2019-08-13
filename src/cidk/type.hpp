#ifndef CIDK_TYPE_HPP
#define CIDK_TYPE_HPP

#include <map>

#include "cidk/def.hpp"

namespace cidk {
  struct Val;
  struct Pos;

  struct Type: Def {
    map<Type *, Type *> parents;
    
    Type(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    void derive(Cx &cx, Type &parent);
    Type *isa(Type &parent) const;
    void mark();
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
