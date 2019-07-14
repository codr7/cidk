#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/str.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::libs {
  static void int_add_imp(Call &call, Env &env, Stack &stack) {
    auto &p(call.pos);
    auto y(pop(p, stack));
    Val &x(stack.back());
    x.as_int += y.as_int;
  }

  static void int_lt_imp(Call &call, Env &env, Stack &stack) {
    auto &cx(env.cx);
    auto &p(call.pos);
    auto y(pop(p, stack));
    Val &x(stack.back());
    x.reset(p, cx.bool_type, x.as_int < y.as_int);
  }

  void init_math(Cx &cx) {
    cx.int_type.env.add_fun(Pos::_,
                            "+",
                            {Arg("x"), Arg("y")},
                            {Ret(cx.int_type)},
                            int_add_imp);

    cx.int_type.env.add_fun(Pos::_,
                            "<",
                            {Arg("x"), Arg("y")},
                            {Ret(cx.bool_type)},
                            int_lt_imp);
  }
}

