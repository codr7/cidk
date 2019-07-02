#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/str.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::libs {
  static void add_imp(Call &call) {
    auto &cx(call.cx);
    auto p(call.pos);
    auto &s(call.cx.stack);

    auto i(s.end()-1);
    Val y(*i--);
    auto &x(*i);
    ValType *xt(x.type), *yt(y.type);
    
    if (xt == yt) {
      s.pop_back();
      xt->add(p, x, y);
    } else {
      xt->env.call(p, cx.intern(str("+/", yt->id)));
    }
  }

  static void lt_imp(Call &call) {
    auto &cx(call.cx);
    auto p(call.pos);
    auto &s(call.cx.stack);

    auto i(s.end()-1);
    Val y(*i--);
    auto &x(*i);
    ValType *xt(x.type), *yt(y.type);
    
    if (xt == yt) {
      s.pop_back();
      x.reset(p, cx.bool_type, xt->lt(p, x, y));
    } else {
      xt->env.call(p, cx.intern(str("</", yt->id)));
    }
  }

  static void dec_imp(Call &call) {
    call.cx.stack.back().as_int--;
  }

  void init_math(Cx &cx) {
    cx.env.add_fun(Pos::_, "+", {Arg("x"), Arg("y")}, {Ret(cx.num_type)}, add_imp);
    cx.env.add_fun(Pos::_, "<", {Arg("x"), Arg("y")}, {Ret(cx.bool_type)}, lt_imp);
    
    cx.env.add_fun(Pos::_, "dec", {Arg("v")}, {Ret(cx.int_type)}, dec_imp);
  }
}

