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
  typename vector<T>::iterator bsearch(const Pos &pos,
                                       vector<T> &items,
                                       const KeyT &key,
                                       Cmp<KeyT, T> cmp = cidk::cmp<KeyT, T>) {
    typename vector<T>::iterator beg(items.begin());
    size_t min(0), max(items.size());
    
    while (min < max) {
      size_t i((min + max) / 2);
      typename vector<T>::iterator it(beg + i);
      
      switch (cmp(pos, key, *it)) {
      case -1:
        max = i;
        break;
      case 1:
        min = i + 1;
        break;
      default:
        return it;
      }
    }

    return beg + max;
  }
}

#endif
