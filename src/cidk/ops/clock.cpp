#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/clock.hpp"
#include "cidk/read.hpp"
#include "cidk/timer.hpp"

namespace cidk::ops {
  struct ClockData {
    Val nreps, body;
    
    ClockData(const Val &nreps, const Val &body): nreps(nreps), body(body) {}
  };
  
  const ClockType Clock("clock");

  ClockType::ClockType(const string &id): OpType(id) {}

  void ClockType::init(Cx &cx, Op &op, const Val &nreps, const Val &body) const {
    op.data = ClockData(nreps, body);
  }

  void ClockType::compile(Cx &cx,
                          OpIter &in,
                          const OpIter &end,
                          Env &env,
                          Stack &stack,
                          Ops &out,
                          Opts &opts) const {
    auto &d(in->as<ClockData>());
    d.nreps.compile(cx, in->pos, env, stack, opts);
    d.body.compile(cx, in->pos, env, stack, opts);
    out.push_back(*in);
  }
  
  void ClockType::eval(Cx &cx, Op &op, Env &env, Reg *regs, Stack &stack) const {
    auto &p(op.pos);
    auto &d(op.as<ClockData>());
    Stack bs;

    d.nreps.push(cx, op.pos, env, regs, stack);
    auto nreps(pop(op.pos, stack));

    if (nreps.type != &cx.int_type) {
      throw ESys(p, "Invalid nreps: ", nreps.type->id);
    }

    Timer t;
    for (int i(0); i < nreps.as_int; i++) { d.body.push(cx, op.pos, env, regs, bs); }
    stack.emplace_back(cx.int_type, Int(t.ms()));
  }

  void ClockType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<ClockData>());
    d.nreps.get_ids(out);
    d.body.get_ids(out);
  }

  void ClockType::mark_refs(Op &op) const {
    auto &d(op.as<ClockData>());
    d.nreps.mark_refs();
    d.body.mark_refs();
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
