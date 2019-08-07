#include "cidk/call.hpp"
#include "cidk/cx.hpp"
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

  void Call::eval(Cx &cx, Env &env, Stack &stack) {
    auto imp(fun.imp);
    
    if (imp) { imp(cx, pos, fun, env, stack); }
    else {
      Reg *regs(cx.regp);
      cx.regp += fun.body_opts.regs.size();

      for (auto &src: fun.body_opts.ext_ids) { 
        set_reg(regs, src.dst_reg, src.id, src.val);
      }
    recall:
      cx.eval(fun.body, fun.env, regs, stack);
      
      if (cx.eval_state == EvalState::recall) {
        cx.eval_state = EvalState::go;
        goto recall;
      }

      cx.regp = regs;
    }
  }
}
