#ifndef CIDK_LIB_MATH_HPP
#define CIDK_LIB_MATH_HPP

namespace cidk {
  struct Cx;

  inline void int_add_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    auto &y(cx.pop(p));
    cx.peek(p).as_int += y.as_int;
  }

  inline void int_sub_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    auto &y(cx.pop(p));
    cx.peek(p).as_int -= y.as_int;
  }

  inline void int_mul_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    auto &y(cx.pop(p));
    cx.peek(p).as_int *= y.as_int;
  }

  inline void int_div_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    auto &y(cx.pop(p));
    cx.peek(p).as_int /= y.as_int;
  }

}

namespace cidk::libs {  
  void init_math(Cx &cx);
}

#endif
