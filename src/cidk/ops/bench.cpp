#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/bench.hpp"
#include "cidk/read.hpp"
#include "cidk/timer.hpp"

namespace cidk::ops {
  struct Data {
    Val nreps, body;
    
    Data(const Val &nreps, const Val &body): nreps(nreps), body(body) { }
  };
  
  const BenchType Bench("bench");

  BenchType::BenchType(string id): OpType(id) { }

  void BenchType::init(Op &op, const Val &nreps, const Val &body) const {
    op.data = Data(nreps, body);
  }
  
  void BenchType::eval(const Op &op, Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    const Pos &p(op.pos);
    Data d(op.as<Data>());
    Stack bs;

    d.nreps.eval(op.pos, env, stack);
    auto nreps(*pop(op.pos, stack, false));

    if (nreps.type != &cx.int_type) {
      throw ESys(p, "Invalid nreps: ", nreps.type->id);
    }

    Timer t;
    for (int i(0); i < nreps.as_int; i++) { d.body.eval(op.pos, env, bs); }
    stack.emplace_back(p, cx.int_type, Int(t.ms()));
  }

  void BenchType::get_ids(const Op &op, IdSet &out) const {
    Data d(op.as<Data>());
    d.nreps.get_ids(out);
    d.body.get_ids(out);
  }

  void BenchType::read(Cx &cx, Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    Pos p(pos);

    auto nreps(read_val(pos, in, env, stack));
    if (!nreps) { throw ESys(p, "Missing bench nreps"); }

    auto body(read_val(pos, in, env, stack));
    if (!body) { throw ESys(p, "Missing bench body"); }
    read_eop(pos, in, env, stack);
    
    out.emplace_back(p, *this, *nreps, *body);
  }
}
