#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/str.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::libs {
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
  }
}

