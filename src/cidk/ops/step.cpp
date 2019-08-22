#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/step.hpp"
#include "cidk/read.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/types/reg.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  const StepType Step("step");

  StepType::StepType(const string &id): OpType(id) {}

  void StepType::init(Cx &cx, Op &op, const Val &n, const Val &delta) const {
    op.args[0] = n;
    op.args[1] = delta;
  }

  void StepType::compile(Cx &cx,
                        OpIter &in,
                        const OpIter &end,
                        Env &env,
                        Ops &out,
                        Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);
    for (int i(0); i < 2; i++) { args[i].compile(p, env, opts); }
    auto tid(args[1].type->id);
    args[2] = env.get(p, cx.intern(p, str("+[", tid, ' ', tid, ']')));
    out.push_back(*in);
  }

  void StepType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);
    auto &n(args[0]), &delta(args[1]);
    
    if (n.type == &cx.reg_type) {
      Val &nv(regs[n.as_reg]);
      cx.push(p, nv);
      cx.push(p, delta);
      args[2].as_fun->call(cx, p, env);
      nv = cx.peek(p);
    } else {
      n.eval(p, env, regs); 
      cx.push(p, delta);      
      args[2].as_fun->call(cx, p, env);
    }
  }

  void StepType::mark_refs(Op &op) const {
    auto &args(op.args);
    for (int i(0); i < 2; i++) { args[i].mark_refs(); }
  }

  void StepType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto n(read_val(cx, pos, in));
    if (!n) { throw ESys(p, "Missing ;"); }
    Val one(cx.int_type, Int(1));
    
    if (n->is_eop()) {
      out.emplace_back(cx, p, *this, cx.$, one);
    } else {
      auto delta(read_val(cx, pos, in));
      if (!delta) { throw ESys(p, "Missing ;"); }
      
      if (delta->is_eop()) {
        out.emplace_back(cx, p, *this, *n, one);
      } else {
        out.emplace_back(cx, p, *this, *n, *delta);
        read_eop(pos, in);
      }
    }
  }
}
