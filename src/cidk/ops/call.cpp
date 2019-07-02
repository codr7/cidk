#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/call.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  const CallType Call("call");

  CallType::CallType(string id): OpType(id) { }

  void CallType::init(Op &op, const Val &target) const { op.data = target; }
  
  void CallType::eval(const Op &op, Env &env) const {
    Cx &cx(env.cx);
    Pos p(op.pos);
    op.as<Val>().eval(p, env);
    auto f(*pop(p, cx.stack, false));
    Type *ft(f.type);
    if (ft != &cx.fun_type) { throw EWrongType(p, "Invalid call target: ", ft); }
    cidk::Call(cx, op.pos, *f.as_fun).eval();
  }

  void CallType::read(Cx &cx, const Pos &pos, Reader &in, Env &env, Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;; n++) {
      auto v(in.read_val(env));
      if (!v) { throw ERead(p, "Missing ;"); }
      if (v->is_eop()) { break; }

      if (v->type != &cx.pop_type) {
        v->eval(p, env);
        Val fv(*pop(p, cx.stack, false));
      
        if (fv.type != &cx.fun_type) {
          throw EWrongType(p, "Invalid call target: ", fv.type);
        }

        v.emplace(fv);
      }

      out.emplace_back(p, *this, *v);
    }

    if (!n) { out.emplace_back(p, *this, Val(cx.pop_type)); }
  }
}
