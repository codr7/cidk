#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/str.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/fix.hpp"

namespace cidk::libs {
  static void int_add_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    const Int y(cx.pop(p).as_int);
    cx.peek(p).as_int += y;
  }

  static void int_sub_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    const Int y(cx.pop(p).as_int);
    cx.peek(p).as_int -= y;
  }

  static void int_mul_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    const Int y(cx.pop(p).as_int);
    cx.peek(p).as_int *= y;
  }

  static void int_div_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    const Int y(cx.pop(p).as_int);
    cx.peek(p).as_int /= y;
  }

  static void fix_add_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    const Fix y(cx.pop(p).as_fix);
    Fix &x(cx.peek(p).as_fix);
    x = fix::add(x, y);
  }

  static void fix_int_mul_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    const Int y(cx.pop(p).as_int);
    Fix &x(cx.peek(p).as_fix);
    x = fix::make(fix::get(x) * y, fix::scale(x));
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
                   "+",
                   {Arg("x", cx.fix_type), Arg("y", cx.fix_type)},
                   {Ret(cx.fix_type)},
                   fix_add_imp);

    cx.env.add_fun(cx, Pos::_,
                   "*",
                   {Arg("x", cx.fix_type), Arg("y", cx.int_type)},
                   {Ret(cx.fix_type)},
                   fix_int_mul_imp);
  }
}

