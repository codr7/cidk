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
  
  void CallType::eval(Cx &cx, const Op &op) const {
    auto f(op.as<Fun *>());

    if (!f) {
      auto fv(cx.stack.back());

      if (fv.type != &cx.fun_type) {
        throw WrongType(op.pos, "Invalid call target: ", fv.type);
      }

      cx.stack.pop_back();
      f = fv.as_fun;
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
