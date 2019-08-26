#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/libs/abc.hpp"
#include "cidk/ops/stash.hpp"
#include "cidk/str.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::libs {
  static void eq_imp(Cx &cx,
                     const Pos &p,
                     const Fun &f,
                     Env &env) {
    auto &y(cx.pop(p));
    Val &x(cx.peek(p));
    x.reset(cx.bool_type, x.eq(p, y));
  }

  static void neq_imp(Cx &cx,
                      const Pos &p,
                      const Fun &f,
                      Env &env) {
    auto &y(cx.pop(p));
    Val &x(cx.peek(p));
    x.reset(cx.bool_type, !x.eq(p, y));
  }

  static void lt_imp(Cx &cx,
                     const Pos &p,
                     const Fun &f,
                     Env &env) {
    auto &y(cx.pop(p));
    Val &x(cx.peek(p));
    x.reset(cx.bool_type, x.cmp(p, y) == -1);
  }

  static void gt_imp(Cx &cx,
                     const Pos &p,
                     const Fun &f,
                     Env &env) {
    auto &y(cx.pop(p));
    auto &x(cx.peek(p));
    x.reset(cx.bool_type, x.cmp(p, y) == 1);
  }

  static void int_add_imp(Cx &cx, const Pos &p, const Fun &f, Env &env) {
    Int y(cx.pop(p).as_int);
    cx.peek(p).as_int += y;
  }
  
  void init_abc(Cx &cx) {
    auto &e(cx.env);

    e.add_const(cx, Pos::_, "$", cx.$);
    e.add_const(cx, Pos::_, "T", cx.T);
    e.add_const(cx, Pos::_, "F", cx.F);   

    e.add_const_expr(cx, Pos::_, "stack", {Op(cx, Pos::_, ops::Stash)});

    e.add_fun(cx, Pos::_,
              "=",
              {Arg("x", cx.a_type), Arg("y", cx.a_type)},
              {Ret(cx.bool_type)},
              eq_imp);

    e.add_fun(cx, Pos::_,
              "!=",
              {Arg("x", cx.a_type), Arg("y", cx.a_type)},
              {Ret(cx.bool_type)},
              neq_imp);
        
    e.add_fun(cx, Pos::_,
              "<",
              {Arg("x", cx.a_type), Arg("y", cx.a_type)},
              {Ret(cx.bool_type)},
              lt_imp);

    e.add_fun(cx, Pos::_,
              ">",
              {Arg("x", cx.a_type), Arg("y", cx.a_type)},
              {Ret(cx.bool_type)},
              gt_imp);

    cx.env.add_fun(cx, Pos::_,
                   "+",
                   {Arg("x", cx.int_type), Arg("y", cx.int_type)},
                   {Ret(cx.int_type)},
                   int_add_imp);
  }
}

