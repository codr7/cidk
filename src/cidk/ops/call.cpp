#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/call.hpp"
#include "cidk/read.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  const CallType Call("call");

  CallType::CallType(string id): OpType(id) { }

  void CallType::init(Op &op, const Val &target) const { op.data = target; }
  
  void CallType::eval(const Op &op, Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    Pos p(op.pos);
    op.as<Val>().eval(p, env, stack);
    auto f(*pop(p, stack, false));
    Type *ft(f.type);
    if (ft != &cx.fun_type) { throw ESys(p, "Invalid call target: ", ft->id); }
    cidk::Call(cx, op.pos, *f.as_fun).eval(env, stack);
  }

  void CallType::get_ids(const Op &op, IdSet &out) const {
    op.as<Val>().get_ids(out);
  }

  void CallType::read(Cx &cx, Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;; n++) {
      auto v(read_val(pos, in, env, stack));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }

      if (v->type != &cx.pop_type) {
        v->eval(p, env, stack);
        Val fv(*pop(p, stack, false));
      
        if (fv.type != &cx.fun_type) {
          throw ESys(p, "Invalid call target: ", fv.type->id);
        }

        v.emplace(fv);
      }

      out.emplace_back(p, *this, *v);
    }

    if (!n) { out.emplace_back(p, *this, Val(cx.pop_type)); }
  }
}
