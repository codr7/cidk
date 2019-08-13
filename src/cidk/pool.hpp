#ifndef CIDK_POOL_HPP
#define CIDK_POOL_HPP

#include <vector>

#include "cidk/cidk.hpp"
#include "cidk/def.hpp"

namespace cidk {
  template <typename T>
  struct Pool {
    using Slot = typename aligned_storage<sizeof(T), alignof(T)>::type;

    size_t slab_size, slab_offs;
    vector<Slot *> slabs;
    vector<T *> free;
    
    Pool(): slab_size(CIDK_POOL_MIN), slab_offs(0) {
      slabs.emplace_back(new Slot[slab_size]);
    }

    ~Pool() {
      for (Slot *i: slabs) { delete[] i; }        
    }
    
    template <typename...Args>
    T *get(Args &&...args) {
      return new (alloc()) T(forward<Args>(args)...);
    }

    T *alloc() {
      T *p(nullptr);
      
      if (free.empty()) {
        Slot *s(nullptr);
        
        if (slab_offs < slab_size) {
          s = slabs.back();
        } else {
          slab_size *= 2;
          s = new Slot[slab_size];
          slabs.push_back(s);
          slab_offs = 0;
        }
      
        p = reinterpret_cast<T *>(&s[slab_offs++]);
      } else {
        p = free.back();
        free.pop_back();
      }
      
      return p;
    }

    void put(T *item) {
      item->~T();
      free.push_back(item);
    }
  };
}

#endif
