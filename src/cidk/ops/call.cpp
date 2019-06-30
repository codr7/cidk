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
    auto p(pos);
    auto v(in.read_val());
    Fun *f(nullptr);
    
    if (v) {
      Val fv;
      in.env.get(p, v->as_sym, fv, false);
      
      if (fv.type != &cx.fun_type) {
        throw WrongType(p, "Invalid call target: ", fv.type);
      }

      f = fv.as_fun;
    }

    out.emplace_back(p, *this, f);
  }
}
