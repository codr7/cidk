#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/expr.hpp"
#include "cidk/fun.hpp"

namespace cidk {
  void Fun::mark() {
    if (!ref_mark) {
      ref_mark = true;
      env.mark();
      mark_refs(body);
      body_opts.mark_refs();
    }
  }  

  void Fun::sweep(Cx &cx, const Pos &pos) { cx.fun_type.pool.put(this); }
}
