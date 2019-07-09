#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/expr.hpp"

namespace cidk {  
  Call::Call(Cx &cx, const Pos &pos, Fun &target):
    cx(cx), pos(pos), prev(cx.call), target(target) {
    cx.call = this;
  }

  Call::~Call() noexcept(false) {
    if (cx.call != this) { throw ESys(pos, "Call ended out of order"); }
    cx.call = prev;
  }

  void Call::eval(Env &env, Stack &stack) {
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
        cx.refs.erase(e.ref_it);
        e.sweep_items(pos);
        e.sweep(pos);
      }
    }
  }
}
