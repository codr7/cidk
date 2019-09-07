#ifndef CIDK_REF_HPP
#define CIDK_REF_HPP

#include "cidk/ls.hpp"

namespace cidk {
  using namespace std;

  struct Cx;
  struct Pos;

  struct Ref: Ls<Ref> {    
    bool ref_mark;

    Ref(Cx &cx);
    virtual ~Ref() = default;
    
    virtual bool mark_refs() { return ref_mark ? false : (ref_mark = true); }
    virtual void sweep(Cx &cx, const Pos &pos) = 0;
  };
}

#endif
