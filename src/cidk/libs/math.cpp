#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/math.hpp"
#include "cidk/str.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/fix.hpp"

namespace cidk::libs {
  static bool int_sub_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    Int y(cx.pop(p).as_int);
    cx.peek(p).as_int -= y;
    return true;
  }

  static bool int_mul_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    Int y(cx.pop(p).as_int);
    cx.peek(p).as_int *= y;
    return true;
  }

  static bool int_div_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    Int y(cx.pop(p).as_int);
    cx.peek(p).as_int /= y;
    return true;
  }

  static bool int_pow_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    Int e(cx.pop(p).as_int);
    if (e < 0) { throw ESys(p, "Negative exponent"); }
    Int &b(cx.peek(p).as_int);
    b = pow(b, e);
    return true;
  }

  static bool int_sqrt_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    auto &x(cx.peek(p).as_int);
    if (x < 0) { throw ESys(p, "Negative argument"); }
    x = sqrt(x);
    return true;
  }

  static bool fix_add_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    Fix y(cx.pop(p).as_fix), &x(cx.peek(p).as_fix);
    x = fix::add(x, y);
    return true;
  }

  static bool fix_sub_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    Fix y(cx.pop(p).as_fix), &x(cx.peek(p).as_fix);
    x = fix::sub(x, y);
    return true;
  }

  static bool fix_mul_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    Fix y(cx.pop(p).as_fix), &x(cx.peek(p).as_fix);
    x = fix::mul(x, y);
    return true;
  }

  static bool fix_div_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    Fix y(cx.pop(p).as_fix), &x(cx.peek(p).as_fix);
    x = fix::div(x, y);
    return true;
  }

  static bool fix_sqrt_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    Fix &x(cx.peek(p).as_fix);
    if (fix::is_neg(x)) { throw ESys(p, "Negative argument"); }
    uint8_t xs(fix::scale(x));
    x = fix::make(sqrt(fix::get(x) * fix::pow(xs)), xs);
    return true;
  }
  
  static bool fix_int_mul_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    const Int y(cx.pop(p).as_int);
    Fix &x(cx.peek(p).as_fix);
    x = fix::make(fix::get(x) * y, fix::scale(x));
    return true;
  }

  static bool fix_int_pow_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    const Int e(cx.pop(p).as_int);
    if (e < 0) { throw ESys(p, "Negative exponent"); }
    Fix &b(cx.peek(p).as_fix);
    uint8_t bs(fix::scale(b));
    b = fix::make(pow(fix::get(b), e) / pow(10, bs), bs);
    return true;
  }

  Lib &init_math(Cx &cx, const Pos &pos) {
    Lib &l(cx.env.add_lib(cx, pos, "math"));
    auto &e(l.env);

    e.add_fun(cx, pos,
              "-",
              {Arg("x", cx.int_type), Arg("y", cx.int_type)},
              {Ret(cx.int_type)},
              int_sub_imp);

    e.add_fun(cx, pos,
              "*",
              {Arg("x", cx.int_type), Arg("y", cx.int_type)},
              {Ret(cx.int_type)},
              int_mul_imp);

    e.add_fun(cx, pos,
              "/",
              {Arg("x", cx.int_type), Arg("y", cx.int_type)},
              {Ret(cx.int_type)},
              int_div_imp);

    e.add_fun(cx, pos,
              "^",
              {Arg("base", cx.int_type), Arg("exp", cx.int_type)},
              {Ret(cx.int_type)},
              int_pow_imp);

    e.add_fun(cx, pos,
              "sqrt",
              {Arg("x", cx.int_type)},
              {Ret(cx.int_type)},
              int_sqrt_imp);


    e.add_fun(cx, pos,
              "+",
              {Arg("x", cx.fix_type), Arg("y", cx.fix_type)},
              {Ret(cx.fix_type)},
              fix_add_imp);

    e.add_fun(cx, pos,
              "-",
              {Arg("x", cx.fix_type), Arg("y", cx.fix_type)},
              {Ret(cx.fix_type)},
              fix_sub_imp);

    e.add_fun(cx, pos,
              "*",
              {Arg("x", cx.fix_type), Arg("y", cx.fix_type)},
              {Ret(cx.fix_type)},
              fix_mul_imp);

    e.add_fun(cx, pos,
              "/",
              {Arg("x", cx.fix_type), Arg("y", cx.fix_type)},
              {Ret(cx.fix_type)},
              fix_div_imp);
    

    e.add_fun(cx, pos,
              "sqrt",
              {Arg("x", cx.fix_type)},
              {Ret(cx.fix_type)},
              fix_sqrt_imp);

    
    e.add_fun(cx, pos,
              "*",
              {Arg("x", cx.fix_type), Arg("y", cx.int_type)},
              {Ret(cx.fix_type)},
              fix_int_mul_imp);

    e.add_fun(cx, pos,
              "^",
              {Arg("base", cx.fix_type), Arg("exp", cx.int_type)},
              {Ret(cx.fix_type)},
              fix_int_pow_imp);

    return l;
  }
}

