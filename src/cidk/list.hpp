#ifndef CIDK_LIST_HPP
#define CIDK_LIST_HPP

#include <deque>

#include "cidk/ref.hpp"

namespace cidk {
  struct Val;
  
  struct List: Ref {
    deque<Val> items;

    List(Cx &cx);

    template <typename SrcT>
    List(Cx &cx, const SrcT &src): Ref(cx), items(src.begin(), src.end()) { }
    
    void mark(const Pos &pos);
    virtual void sweep(const Pos &pos) override;
  };
}

#endif
