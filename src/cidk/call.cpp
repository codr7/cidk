#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/expr.hpp"

namespace cidk {  
  Call::Call(const Pos &pos, Fun &fun):
    pos(pos), prev(fun.cx.call), fun(fun) {
    fun.cx.call = this;
  }

  Call::~Call() noexcept(false) {
    Call *&c(fun.cx.call);
    if (c != this) { throw ESys(pos, "Call ended out of order"); }
    c = prev;
  }

  void Call::eval(Fun &fun, Env &env, Stack &stack) {
    Cx &cx(env.cx);
    auto imp(fun.imp);
    
    if (imp) { imp(*this, env, stack); }
    else {
      const ReadState &opt(fun.body_opts);
      Env &e(opt.env_extend ? *cx.env_pool.get(fun.env) : fun.env);
    recall:
      cx.eval(fun.body, e, stack);
      
      if (cx.eval_state == EvalState::recall) {
        if (opt.env_extend) { e.restore(pos, fun.env, !opt.env_escape); }
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
  
  void Call::eval(Env &env, Stack &stack) { eval(fun, env, stack); }

  void Call::forward(Env &dst, const Sym *id, Env &env, Stack &stack) {
    Cx &cx(env.cx);
    Val &v(dst.get(pos, id));
    Type *vt(v.type);
    if (vt != &cx.fun_type) { throw ESys(pos, "Expected Fun, was: ", vt->id); }
    eval(*v.as_fun, env, stack);
  }
}
