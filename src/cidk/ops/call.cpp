#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/call.hpp"

namespace cidk::ops {
  const CallType Call("call");

  CallType::CallType(string id): OpType(id) { }

  void CallType::init(Op &op, Fun *fun) const {
    op.data = fun;
  }
  
  void CallType::eval(const Op &op, Env &env) const {
    Cx &cx(env.cx);
    Pos p(op.pos);
    Fun *f(op.as<Fun *>());

    if (!f) {
      Stack &s(cx.stack);
      auto fv(pop(p, s, false));
      Type *ft(fv->type);
      if (ft != &cx.fun_type) { throw WrongType(p, "Invalid call target: ", ft); }
      f = fv->as_fun;
    }
    
    cidk::Call(cx, op.pos, *f).eval();
  }

  void CallType::read(Cx &cx, const Pos &pos, Reader &in, Ops &out) const {
    for (;;) {
      Pos p(pos);
      auto v(in.read_val());
      if (!v) { throw ReadE(p, "Missing ;"); }
      if (v->is_eol()) { break; }

      v->eval(in.env);
      Val fv(*pop(p, cx.stack, false));
      
      if (fv.type != &cx.fun_type) {
        throw WrongType(p, "Invalid call target: ", fv.type);
      }

      out.emplace_back(p, *this, fv.as_fun);
    }
  }
}
