#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/expr.hpp"
#include "cidk/fun.hpp"

namespace cidk {
  void Fun::call(Cx &cx, const Pos &pos, Env &env) {
    if (imp) {
      imp(cx, pos, *this, env);
    } else {
      Call(pos, *this).eval(cx);
    }
  }
  
  void Fun::mark() {
    if (!ref_mark) {
      ref_mark = true;
      env.mark_refs();
      mark_refs(body);
      body_opts.mark_refs();
    }
  }  

  void Fun::sweep(Cx &cx, const Pos &pos) { cx.fun_type.pool.put(this); }
}
