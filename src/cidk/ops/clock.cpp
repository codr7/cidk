#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/clock.hpp"
#include "cidk/read.hpp"
#include "cidk/timer.hpp"

namespace cidk::ops {
  struct Data {
    Val nreps, body;
    
    Data(const Val &nreps, const Val &body): nreps(nreps), body(body) { }
  };
  
  const ClockType Clock("clock");

  ClockType::ClockType(const string &id): OpType(id) { }

  void ClockType::init(Op &op, const Val &nreps, const Val &body) const {
    op.data = Data(nreps, body);
  }
  
  void ClockType::eval(const Op &op, Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    const Pos &p(op.pos);
    const Data &d(op.as<Data>());
    Stack bs;

    d.nreps.eval(op.pos, env, stack);
    auto nreps(pop(op.pos, stack));

    if (nreps.type != &cx.int_type) {
      throw ESys(p, "Invalid nreps: ", nreps.type->id);
    }

    Timer t;
    for (int i(0); i < nreps.as_int; i++) { d.body.eval(op.pos, env, bs); }
    stack.emplace_back(p, cx.int_type, Int(t.ms()));
  }

  void ClockType::get_ids(const Op &op, IdSet &out) const {
    Data d(op.as<Data>());
    d.nreps.get_ids(out);
    d.body.get_ids(out);
  }

  void ClockType::read(Cx &cx, Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    Pos p(pos);

    auto nreps(read_val(pos, in, env, stack));
    if (!nreps) { throw ESys(p, "Missing clock nreps"); }

    auto body(read_val(pos, in, env, stack));
    if (!body) { throw ESys(p, "Missing clock body"); }
    read_eop(pos, in, env, stack);
    
    out.emplace_back(p, *this, *nreps, *body);
  }
}
