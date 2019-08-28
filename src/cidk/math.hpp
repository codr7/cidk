#ifndef CIDK_MATH_HPP
#define CIDK_MATH_HPP

#include <cstdint>

namespace cidk {
  constexpr int64_t pow(int64_t b, uint64_t e) {
    int64_t r(1);

    for (;;) {
      if (e & 1) { r *= b; }
      e >>= 1;
      if (!e) { break; }
      b *= b;
    }

    return r;
  }

  constexpr uint64_t sqrt(uint64_t n) {
    uint64_t s(2), ns(n >> s);
    
    while (ns && ns != n) {
      s += 2;
      ns = n >> s;
    }
    
    s -= 2;
    uint64_t r(0);
    
    for (;;) {
      r <<= 1;
      uint64_t cr(r + 1);
      if (cr * cr <= n >> s) { r = cr; }
      if (!s) { break; }
      s -= 2;
    }
    
    return r;
  }
}

#endif
