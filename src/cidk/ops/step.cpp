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
    out.push_back(*in);
  }

  void StepType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);
    auto &n(args[0]), &delta(args[1]);
    
    if (n.type == &cx.reg_type) {
      delta.eval(p, env, regs);
      auto &dv(cx.peek(p));
      
      if (dv.type != &cx.int_type) {
        throw ESys(p, "Expected Int, was: ", dv.type->id);
      }

      auto &nv(regs[n.as_reg].as_int);
      nv += dv.as_int;
      dv.as_int = nv;
    } else {
      n.eval(p, env, regs); 
      delta.eval(p, env, regs);
      auto &dv(cx.pop(p));
      
      if (dv.type != &cx.int_type) {
        throw ESys(p, "Expected Int, was: ", dv.type->id);
      }

      cx.peek(p).as_int += dv.as_int;
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
