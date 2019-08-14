#ifndef CIDK_LIST_HPP
#define CIDK_LIST_HPP

#include <vector>

#include "cidk/ref.hpp"

namespace cidk {
  struct Val;
  
  struct List: Ref {
    vector<Val> items;

    List(Cx &cx);

    template <typename SrcT>
    List(Cx &cx, const SrcT &src): Ref(cx), items(src.begin(), src.end()) {}
    
    void mark();
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
