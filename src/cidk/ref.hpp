#ifndef CIDK_REF_HPP
#define CIDK_REF_HPP

#include "cidk/ls.hpp"

namespace cidk {
  using namespace std;

  struct Cx;
  struct Pos;

  struct CxRefs {};

  enum struct RefState {keep, skip, sweep};
  
  struct Ref: Ls<Ref, CxRefs> {    
    RefState ref_state;

    Ref(Cx &cx);
    virtual ~Ref() = default;
    
    virtual void sweep(Cx &cx, const Pos &pos) = 0;
  };
}

#endif
