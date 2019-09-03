#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/lib.hpp"
#include "cidk/libs/abc.hpp"
#include "cidk/ops/stash.hpp"
#include "cidk/str.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::libs {
  static bool eq_imp(Cx &cx,
                     const Pos &p,
                     const Fun &f,
                     Env &env) {
    auto &y(cx.pop(p));
    Val &x(cx.peek(p));
    x.reset(cx.bool_type, x.eq(p, y));
    return true;
  }

  static bool neq_imp(Cx &cx,
                      const Pos &p,
                      const Fun &f,
                      Env &env) {
    auto &y(cx.pop(p));
    Val &x(cx.peek(p));
    x.reset(cx.bool_type, !x.eq(p, y));
    return true;
  }

  static bool lt_imp(Cx &cx,
                     const Pos &p,
                     const Fun &f,
                     Env &env) {
    auto &y(cx.pop(p));
    Val &x(cx.peek(p));
    x.reset(cx.bool_type, x.cmp(p, y) == -1);
    return true;
  }

  static bool gt_imp(Cx &cx,
                     const Pos &p,
                     const Fun &f,
                     Env &env) {
    auto &y(cx.pop(p));
    auto &x(cx.peek(p));
    x.reset(cx.bool_type, x.cmp(p, y) == 1);
    return true;
  }

  static bool int_add_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    Int y(cx.pop(p).as_int);
    cx.peek(p).as_int += y;
    return true;
  }
  
  Lib &init_abc(Cx &cx, const Pos &pos) {
    Lib &l(cx.env.add_lib(cx, pos, "abc"));
    auto &e(l.env);

    e.add_const(cx, pos, "$", cx.$);
    e.add_const(cx, pos, "T", cx.T);
    e.add_const(cx, pos, "F", cx.F);   

    e.add_const_expr(cx, pos, "stack", {Op(cx, pos, ops::Stash)});

    e.add_fun(cx, pos,
              "=",
              {Arg("x", cx.a_type), Arg("y", cx.a_type)},
              {Ret(cx.bool_type)},
              eq_imp);

    e.add_fun(cx, pos,
              "!=",
              {Arg("x", cx.a_type), Arg("y", cx.a_type)},
              {Ret(cx.bool_type)},
              neq_imp);
        
    e.add_fun(cx, pos,
              "<",
              {Arg("x", cx.a_type), Arg("y", cx.a_type)},
              {Ret(cx.bool_type)},
              lt_imp);

    e.add_fun(cx, pos,
              ">",
              {Arg("x", cx.a_type), Arg("y", cx.a_type)},
              {Ret(cx.bool_type)},
              gt_imp);

    cx.env.add_fun(cx, pos,
                   "+",
                   {Arg("x", cx.int_type), Arg("y", cx.int_type)},
                   {Ret(cx.int_type)},
                   int_add_imp);

    return l;
  }
}

