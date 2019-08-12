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
  recall:
    cx.eval(fun.body, fun.env, fun.body_opts, cx.regp);
    
    if (cx.eval_state == EvalState::recall) {
      cx.eval_state = EvalState::go;
      goto recall;
    }
  }
}
