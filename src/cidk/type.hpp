#ifndef CIDK_TYPE_HPP
#define CIDK_TYPE_HPP

#include <map>

#include "cidk/cidk.hpp"
#include "cidk/def.hpp"
#include "cidk/int.hpp"

namespace cidk {
  struct Val;
  struct Pos;

  struct Type: Def {
    Int tag;
    array<Type *, CIDK_TYPE_MAX> parents;
    
    Type(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    void derive(Cx &cx, Type &parent);
    Type *isa(Type &parent) const;
    void mark();
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
