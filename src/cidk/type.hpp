#ifndef CIDK_TYPE_HPP
#define CIDK_TYPE_HPP

#include <map>

#include "cidk/cidk.hpp"
#include "cidk/def.hpp"
#include "cidk/int.hpp"

namespace cidk {
  struct Env;
  struct Pos;
  struct Val;

  struct Type: Def {
    Env &env;
    Int tag;
    array<Type *, CIDK_TYPE_MAX> parents;
    Type *nil_type;
    
    Type(Cx &cx,
         const Pos &pos,
         Env &env,
         const Sym *id,
         const vector<Type *> &parents);
    
    void init_parents();

    void derive(Type &parent);
    void derive(Type &parent, Type &root);

    Type *isa(Type &parent) const {
      Type *p(&parent);
      if (p == this) { return p; }
      return parents[parent.tag];
    }

    void mark();
    Type &or_nil(const Pos &pos);
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
