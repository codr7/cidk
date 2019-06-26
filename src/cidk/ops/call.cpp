#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/call.hpp"

namespace cidk::ops {
  const CallType Call("call");

  CallType::CallType(string id): OpType(id) { }

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

  void init(Op &op, const ops::CallType &type, Fun *fun) {
    op.data = fun;
  }
}
