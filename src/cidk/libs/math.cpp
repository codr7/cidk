#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/str.hpp"

namespace cidk::libs {
  static void add_imp(Call &call) {
    auto &cx(call.cx);
    auto p(call.pos);
    auto &s(call.cx.stack);

    auto i(s.end()-1);
    auto y(*i--);
    auto &x(*i);
    ValType *xt(x.type), *yt(y.type);
    
    if (xt == yt) {
      s.pop_back();
      xt->add(p, x, y);
    } else {
      xt->call_env(p, cx.intern(str("+/", yt->id)));
    }
  }

  void init_math(Cx &cx) {
    cx.env.add_fun(Pos::_, "+", {Arg("x"), Arg("y")}, {Ret(cx.num_type)}, add_imp);
  }
}

