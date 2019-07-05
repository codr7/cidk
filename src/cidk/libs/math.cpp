#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/str.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::libs {
  static void add_imp(Call &call, Env &env, Stack &stack) {
    auto &cx(call.cx);
    auto &p(call.pos);

    auto i(stack.end()-1);
    Val &y(*i--), &x(*i);
    ValType *xt(x.type), *yt(y.type);
    
    if (xt == yt) {
      xt->env.call(p, cx.add_id, env, stack);
    } else {
      xt->env.call(p, cx.intern(str("+/", yt->id)), env, stack);
    }
  }

  static void int_add_imp(Call &call, Env &env, Stack &stack) {
    auto &p(call.pos);
    auto y(pop(p, stack, false));
    Val &x(stack.back());
    x.as_int += y->as_int;
  }

  static void lt_imp(Call &call, Env &env, Stack &stack) {
    auto &cx(call.cx);
    auto &p(call.pos);

    auto i(stack.end()-1);
    Val &y(*i--), &x(*i);
    ValType *xt(x.type), *yt(y.type);

    if (xt == yt) {
      xt->env.call(p, cx.lt_id, env, stack);
    } else {
      xt->env.call(p, cx.intern(str("</", yt->id)), env, stack);
    }
  }

  static void int_lt_imp(Call &call, Env &env, Stack &stack) {
    auto &cx(call.cx);
    auto &p(call.pos);
    auto y(pop(p, stack, false));
    Val &x(stack.back());
    x.reset(p, cx.bool_type, x.as_int < y->as_int);
  }

  static void int_dec_imp(Call &call, Env &env, Stack &stack) {
    stack.back().as_int--;
  }

  void init_math(Cx &cx) {
    cx.env.add_fun(Pos::_, "+", {Arg("x"), Arg("y")}, {Ret(cx.any_type)}, add_imp);

    cx.int_type.env.add_fun(Pos::_,
                            "+",
                            {Arg("x"), Arg("y")},
                            {Ret(cx.int_type)},
                            int_add_imp);

    cx.env.add_fun(Pos::_, "<", {Arg("x"), Arg("y")}, {Ret(cx.bool_type)}, lt_imp);

    cx.int_type.env.add_fun(Pos::_,
                            "<",
                            {Arg("x"), Arg("y")},
                            {Ret(cx.bool_type)},
                            int_lt_imp);
    
    cx.env.add_fun(Pos::_, "dec", {Arg("v")}, {Ret(cx.int_type)}, int_dec_imp);
  }
}

