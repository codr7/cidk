#ifndef CIDK_PAIR_HPP
#define CIDK_PAIR_HPP

#include "cidk/ref.hpp"
#include "cidk/val.hpp"

namespace cidk {
  struct Pair: Ref, pair<Val, Val> {
    Pair(Cx &cx, const Val &l, const Val &r): Ref(cx), pair(l, r) {}
    Pair(Cx &cx, const pair<Val, Val> &src): Ref(cx), pair(src) {}

    virtual bool mark_refs() override {
      if (Ref::mark_refs()) {
        first.mark_refs();
        second.mark_refs();
        return true;
      }

      return false;
    }    

    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
