#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/str.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::libs {
  static void int_add_imp(Cx &cx,
                          const Pos &p,
                          const Fun &f,
                          Env &env) {
    auto &y(cx.pop(p));
    cx.peek(p).as_int += y.as_int;
  }

  static void int_sub_imp(Cx &cx,
                          const Pos &p,
                          const Fun &f,
                          Env &env) {
    auto &y(cx.pop(p));
    cx.peek(p).as_int -= y.as_int;
  }

  static void int_mul_imp(Cx &cx,
                          const Pos &p,
                          const Fun &f,
                          Env &env) {
    auto &y(cx.pop(p));
    cx.peek(p).as_int *= y.as_int;
  }

  static void int_div_imp(Cx &cx,
                          const Pos &p,
                          const Fun &f,
                          Env &env) {
    auto &y(cx.pop(p));
    cx.peek(p).as_int /= y.as_int;
  }

  static void int_lt_imp(Cx &cx,
                         const Pos &p,
                         const Fun &f,
                         Env &env) {
    auto &y(cx.pop(p));
    Val &x(cx.peek(p));
    x.reset(cx.bool_type, x.as_int < y.as_int);
  }

  static void int_gt_imp(Cx &cx,
                         const Pos &p,
                         const Fun &f,
                         Env &env) {
    auto &y(cx.pop(p));
    auto &x(cx.peek(p));
    x.reset(cx.bool_type, x.as_int > y.as_int);
  }

  void init_math(Cx &cx) {
    cx.env.add_fun(cx, Pos::_,
                   "+",
                   {Arg("x", cx.int_type), Arg("y", cx.int_type)},
                   {Ret(cx.int_type)},
                   int_add_imp);

    cx.env.add_fun(cx, Pos::_,
                   "-",
                   {Arg("x", cx.int_type), Arg("y", cx.int_type)},
                   {Ret(cx.int_type)},
                   int_sub_imp);

    cx.env.add_fun(cx, Pos::_,
                   "*",
                   {Arg("x", cx.int_type), Arg("y", cx.int_type)},
                   {Ret(cx.int_type)},
                   int_mul_imp);

    cx.env.add_fun(cx, Pos::_,
                   "/",
                   {Arg("x", cx.int_type), Arg("y", cx.int_type)},
                   {Ret(cx.int_type)},
                   int_div_imp);

    cx.env.add_fun(cx, Pos::_,
                   "<",
                   {Arg("x", cx.int_type), Arg("y", cx.int_type)},
                   {Ret(cx.bool_type)},
                   int_lt_imp);

    cx.env.add_fun(cx, Pos::_,
                   ">",
                   {Arg("x", cx.int_type), Arg("y", cx.int_type)},
                   {Ret(cx.bool_type)},
                   int_gt_imp);
  }
}

