#ifndef CIDK_DEFER_HPP
#define CIDK_DEFER_HPP

#define defer1(x, y) x##y
#define defer2(x, y) defer1(x, y)
#define defer3(x)    defer2(x, __COUNTER__)
#define defer(code)  auto defer3(_defer) = make_defer([&]()code)

namespace cidk {
  template <typename F>
  struct Defer {
    F f;
    Defer(F f) : f(f) {}
    ~Defer() { f(); }
  };

  template <typename F>
  Defer<F> make_defer(F f) { return Defer<F>(f); }
}

#endif
