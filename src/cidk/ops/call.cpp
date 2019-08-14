#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/call.hpp"
#include "cidk/read.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  const CallType Call("call");

  CallType::CallType(const string &id): OpType(id) {}

  void CallType::init(Cx &cx, Op &op, const Val &target) const {
    op.args[0] = target;
  }

  void CallType::compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const {
    in->args[0].compile(in->pos, env, opts);
    out.push_back(*in);
  }

  void CallType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &v(op.args[0]);
    Fun *f(nullptr);
    
    if (v.type == &cx.fun_type) {
      f = v.as_fun;
    } else {
      v.eval(p, env, regs);
      auto &fv(cx.pop(p));
      if (fv.type != &cx.fun_type) { throw ESys(p, "Invalid call target: ", fv); }
      f = fv.as_fun;
    }
    
    f->call(cx, p, env);
  }

  void CallType::mark_refs(Op &op) const { op.args[0].mark_refs(); }

  void CallType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;; n++) {
      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      out.emplace_back(cx, p, *this, *v);
    }

    if (!n) { out.emplace_back(cx, p, *this, cx.$); }
  }
}
