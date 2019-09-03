#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/defer.hpp"
#include "cidk/e.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/expr.hpp"

namespace cidk {  
  Call::Call(const Pos &pos, Fun &fun): pos(pos), prev(fun.cx.call), fun(fun) {
    fun.cx.call = this;
  }

  Call::~Call() noexcept(false) {
    Call *&c(fun.cx.call);
    if (c != this) { throw ESys(pos, "Call ended out of order"); }
    c = prev;
  }

  void Call::eval(Cx &cx) {
    auto &opts(fun.body_opts);
  recall:
    Reg *eval_regs(cx.alloc_regs(opts.regs.size()));

    for (auto &eid: opts.ext_ids) {
      set_reg(eval_regs, eid.dst_reg, eid.id, eid.val);
    }

    auto min_defer(cx.deferp);
    cx.eval(fun.body, fun.env, eval_regs);

    if (cx.eval_state == EvalState::recall) {
      cx.eval_state = EvalState::go;
      cx.eval_defers(min_defer, fun.env, eval_regs);
      goto recall;
    }
  }
}
