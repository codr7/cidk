#ifndef CIDK_LIST_HPP
#define CIDK_LIST_HPP

#include <list>

#include "cidk/ref.hpp"

namespace cidk {
  struct Val;
  
  struct List: Ref {
    list<Val> items;
    
    List(Cx &cx);
    virtual void sweep(const Pos &pos) override;
  };
}

#endif
