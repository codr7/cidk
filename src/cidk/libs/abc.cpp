#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/libs/abc.hpp"
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

  void init_abc(Cx &cx) {
    cx.env.add_fun(cx, Pos::_,
                   "=",
                   {Arg("x", cx.any_type), Arg("y", cx.any_type)},
                   {Ret(cx.bool_type)},
                   eq_imp);

    cx.env.add_fun(cx, Pos::_,
                   "!=",
                   {Arg("x", cx.any_type), Arg("y", cx.any_type)},
                   {Ret(cx.bool_type)},
                   neq_imp);
        
    cx.env.add_fun(cx, Pos::_,
                   "<",
                   {Arg("x", cx.any_type), Arg("y", cx.any_type)},
                   {Ret(cx.bool_type)},
                   lt_imp);

    cx.env.add_fun(cx, Pos::_,
                   ">",
                   {Arg("x", cx.any_type), Arg("y", cx.any_type)},
                   {Ret(cx.bool_type)},
                   gt_imp);
  }
}

