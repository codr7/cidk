#ifndef CIDK_FIX_HPP
#define CIDK_FIX_HPP

#include "cidk/cmp.hpp"

namespace cidk::fix {
  const uint8_t SCALE_BITS(4);

  using T = uint64_t;
    
  inline T make(int64_t rep, uint8_t scale) {
    const bool is_neg(rep < 0);
    const uint64_t r(is_neg ? -rep : rep);
    return is_neg + (scale << 1) + (r << (SCALE_BITS + 1));
  }
  
  inline bool is_neg(T f) { return f << 63; }
  
  inline uint8_t scale(T f) { return (f >> 1) & ((1 << SCALE_BITS) - 1); }
  
  inline int64_t get(T f) {
    const uint64_t r(f >> (SCALE_BITS + 1));
    return is_neg(f) ? -r : r;
  }

  inline int64_t pow(uint8_t scale) {
    uint64_t out(1);
    while (scale--) { out *= 10; }
    return out;
  }

  inline int64_t trunc(T f, uint8_t ts = 0) {
    const uint8_t fs(scale(f));
    const uint8_t s((fs < ts) ? fs : fs - ts);
    return get(f) / pow(s);
  }
  
  inline int64_t frac(T f) { return get(f) - trunc(f); }
  
  inline int cmp(const Pos &pos, T x, T y) {
    const int64_t xv(get(x)), yv(get(y));
    const uint8_t xs(scale(x)), ys(scale(y));
    return cidk::cmp(pos, xv, (xs == ys) ? yv : yv / pow(ys) * pow(xs));
  }

  inline void dump(T f, ostream &out) { out << trunc(f) << '.' << frac(f); }
  
  inline T add(T x, T y) {
    const uint8_t xs(scale(x)), ys(scale(y));
    const int64_t yv(get(y));
    return make(get(x) + ((xs == ys) ? yv : yv * pow(xs) / pow(ys)), xs);
  }
  
  inline T sub(T x, T y) {
    const uint8_t xs(scale(x)), ys(scale(y));
    const int64_t yv(get(y));
    return make(get(x) - ((xs == ys) ? yv : yv * pow(xs) / pow(ys)), xs);
  }

  inline T mul(T x, T y) {
    const uint8_t xs(scale(x));
    return make(get(x) * get(y) / pow(scale(y)), xs);
  }

  inline T div(T x, T y) {
    const uint8_t xs(scale(x));
    return make(get(x) / get(y) / pow(scale(y)), xs);
  }
}

namespace cidk { using Fix = fix::T; }

#endif