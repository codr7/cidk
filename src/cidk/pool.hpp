#ifndef CIDK_POOL_HPP
#define CIDK_POOL_HPP

#include <vector>

#include "cidk/conf.hpp"
#include "cidk/def.hpp"

namespace cidk {
  template <typename T>
  struct Pool {
    using Item = typename aligned_storage<sizeof(T), alignof(T)>::type;

    size_t slab_size, slab_offs;
    vector<Item *> slabs;
    vector<T *> free;
    
    Pool(): slab_size(7), slab_offs(0) {
      slabs.emplace_back(new Item[slab_size]);
    }

    ~Pool() {
      for (Item *i: slabs) { delete[] i; }        
    }
    
    template <typename...Args>
    T *get(Args &&...args) {
#ifndef CIDK_USE_POOL
      return new T(forward<Args>(args)...);
#endif
      
      return new (alloc()) T(forward<Args>(args)...);
    }

    T *alloc() {
#ifndef CIDK_USE_POOL
      return reinterpret_cast<T *>(new Item());
#endif
      T *p(nullptr);
      
      if (free.empty()) {
        Item *s(nullptr);
        
        if (slab_offs < slab_size) {
          s = slabs.back();
        } else {
          slab_size *= 2;
          s = new Item[slab_size];
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
#ifdef CIDK_USE_POOL
      item->~T();
      free.push_back(item);
#else
      delete item;
#endif
    }
  };
}

#endif
