#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/expr.hpp"

namespace cidk {  
  Call::Call(const Pos &pos, Fun &target):
    pos(pos), prev(target.cx.call), target(target) {
    target.cx.call = this;
  }

  Call::~Call() noexcept(false) {
    Call *&c(target.cx.call);
    if (c != this) { throw ESys(pos, "Call ended out of order"); }
    c = prev;
  }

  void Call::eval(Env &env, Stack &stack) {
    Cx &cx(env.cx);
    auto imp(target.imp);

    if (imp) { imp(*this, env, stack); }
    else {
      const ReadState &opt(target.body_opts);
      Env &e(opt.env_extend ? *cx.env_pool.get(target.env) : target.env);
    recall:
      cx.eval(target.body, e, stack);
      
      if (cx.eval_state == EvalState::recall) {
        if (opt.env_extend) { e.restore(pos, target.env, !opt.env_escape); }
        cx.eval_state = EvalState::go;
        goto recall;
      }

      if (opt.env_extend && !opt.env_escape) {
        dynamic_cast<Ref &>(e).unlink();
        e.sweep_items(pos);
        e.sweep(cx, pos);
      }
    }
  }
}
