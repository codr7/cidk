#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/clock.hpp"
#include "cidk/read.hpp"
#include "cidk/timer.hpp"

namespace cidk::ops {
  const ClockType Clock("clock");

  ClockType::ClockType(const string &id): OpType(id) {}

  void ClockType::init(Cx &cx, Op &op, const Val &nreps, const Val &body) const {
    op.args[0] = nreps;
    op.args[1] = body;
  }

  void ClockType::compile(Cx &cx, Op &op, Env &env, Ops &out, Opts &opts) const {
    auto &p(op.pos);
    auto &args(op.args);
    for (int i(0); i < 2; i++) { args[i].compile(p, env, opts); }
    out.push_back(op);
  }
  
  bool ClockType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);

    args[0].eval(p, env, regs);
    auto nreps(cx.pop(p));

    if (nreps.type != &cx.int_type) {
      throw ESys(p, "Invalid nreps: ", nreps.type->id);
    }

    auto &body(args[1]);
    
    Timer t;
    auto stackp(cx.stackp);
    
    for (int i(0); i < nreps.as_int; i++) {
      if (!body.eval(p, env, regs)) { return false; }
      cx.stackp = stackp;
    }
    
    cx.push(p, cx.int_type, Int(t.ms()));
    return true;
  }

  bool ClockType::find_op(Op &op, function<bool (Ops &, OpIter &)> pred) const {
    auto &args(op.args);

    for (int i(0); i < 2; i++) {
      if (args[i].find_op(pred)) { return true; }
    }

    return false;
  }

  void ClockType::mark_refs(Op &op) const {
    auto &args(op.args);
    for (int i(0); i < 2; i++) { args[i].mark_refs(); }
  }

  void ClockType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);

    auto nreps(read_val(cx, pos, in));
    if (!nreps || nreps->is_eop()) { throw ESys(p, "Missing clock nreps"); }

    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing clock body"); }
    read_eop(pos, in);
    
    out.emplace_back(cx, p, *this, *nreps, *body);
  }
}
