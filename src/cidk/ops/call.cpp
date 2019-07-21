#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/call.hpp"
#include "cidk/read.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  const CallType Call("call");

  CallType::CallType(const string &id): OpType(id) {}

  void CallType::init(Cx &cx, Op &op, const Val &target) const { op.data = target; }

  void CallType::compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Stack &stack,
                         Ops &out,
                         Opts *opts) const {
    Val &f(in->as<Val>());
    f.compile(cx, in->pos, env, stack, opts);
    Type *ft(f.type);
    if (ft != &cx.fun_type) { throw ESys(in->pos, "Invalid call target: ", ft->id); }
    out.push_back(*in);
  }

  void CallType::eval(Op &op, Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    cidk::Call(op.pos, *op.as<Val>().as_fun).eval(cx, env, stack);
  }

  void CallType::get_ids(const Op &op, IdSet &out) const {
    op.as<Val>().get_ids(out);
  }

  void CallType::mark_refs(Op &op) const { op.as<Val>().mark_refs(); }

  void CallType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;; n++) {
      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      out.emplace_back(cx, p, *this, *v);
    }

    if (!n) { out.emplace_back(cx, p, *this, Val(cx.pop_type)); }
  }
}
