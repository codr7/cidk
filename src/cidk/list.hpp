#ifndef CIDK_LIST_HPP
#define CIDK_LIST_HPP

#include <vector>

#include "cidk/cmp.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  struct Val;
  
  struct List: Ref {
    using Items = vector<Val>;
    using Iter = Items::iterator;
    Items items;

    List(Cx &cx);

    template <typename SrcT>
    List(Cx &cx, const SrcT &src): Ref(cx), items(src.begin(), src.end()) {}

    template <typename SrcT>
    List(Cx &cx, const SrcT &beg, const SrcT &end): Ref(cx), items(beg, end) {}

    void mark();
    void sort(const Pos &pos, Cmp<Val> cmp = cidk::cmp<Val>);
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };

  template <typename T, typename KeyT = T>
  typename vector<T>::iterator find(const Pos &pos,
                                    vector<T> &items,
                                    const KeyT &key,
                                    Cmp<T, KeyT> cmp = cidk::cmp<T, KeyT>) {
    return lower_bound(items.begin(), items.end(), key,
                       [&](auto &x, auto &y) { return cmp(pos, x, y) == -1; });
  }
}

#endif
