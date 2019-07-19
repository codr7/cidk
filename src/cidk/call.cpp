#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
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

  void Call::eval(Cx &cx, Env &env, Stack &stack) {
    auto imp(fun.imp);
    
    if (imp) { imp(cx, pos, fun, env, stack); }
    else {
      const ReadState &opt(fun.body_opts);
      Env &e(opt.env_extend ? *cx.env_pool.get(fun.env) : fun.env);
    recall:
      cx.eval(fun.body, e, stack);
      
      if (cx.eval_state == EvalState::recall) {
        if (opt.env_extend) { e.restore(fun.env); }
        cx.eval_state = EvalState::go;
        goto recall;
      }

      if (opt.env_extend && !opt.env_escape) {
        dynamic_cast<Ref &>(e).unlink();
        e.sweep(cx, pos);
      }
    }
  }
}
