#ifndef CIDK_DEFER_HPP
#define CIDK_DEFER_HPP

namespace cidk {
  template <typename F>
  struct Defer {
    F f;
    Defer(F f) : f(f) {}
    ~Defer() { f(); }
  };

  template <typename F>
  Defer<F> defer(F f) { return Defer<F>(f); }
}

#endif
