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
      auto v(cx.stack.back());

      if (v.type != &cx.Fun) {
        throw WrongType(op.pos, "Invalid call target: ", v.type);
      }

      cx.stack.pop_back();
      f = v.as_fun;
    }
    
    cidk::Call(cx, op.pos, *f).eval();
  }

  void CallType::read(Cx &cx, const Pos &pos, Reader &in, Ops &out) const {
    auto p(pos);
    auto v(in.read_val());
    Fun *f(v ? v->as_fun : nullptr);
    out.emplace_back(p, *this, f);
  }
}
