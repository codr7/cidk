#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/expr.hpp"
#include "cidk/ops/assert.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::ops {
  struct AssertData {
    Val args, body;
    AssertData(const Val &args, const Val &body): args(args), body(body) {}
  };

  const AssertType Assert("assert");

  AssertType::AssertType(const string &id): OpType(id) {}

  void AssertType::init(Op &op, const Val &args, const Val &body) const {
    op.data = AssertData(args, body);
  }

  void AssertType::eval(const Op &op, Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    const Pos &p(op.pos);
    const auto &d(op.as<AssertData>());
    Stack arg_stack(stack);
    d.args.eval(p, env, arg_stack);
    Val args(pop(p, arg_stack));
    
    d.body.eval(p, env, stack);
    Val ok(pop(op.pos, stack));
    
    if (ok.type != &cx.bool_type) {
      throw ESys(p, "Expected Bool, was: ", ok.type->id);
    }

    if (!ok.as_bool) { throw ESys(p, "Test failed: ", args); }
  }

  void AssertType::get_ids(const Op &op, IdSet &out) const {
    op.as<Val>().get_ids(out);
  }

  void AssertType::mark_refs(Op &op) const { op.as<Val>().mark_refs(); }

  void AssertType::read(Cx &cx,
                        Pos &pos,
                        istream &in,
                        ReadState &state,
                        Env &env,
                        Stack &stack,
                        Ops &out) const {
    auto args(read_val(pos, in, state, env, stack));
    auto body(read_val(pos, in, state, env, stack));
    read_eop(pos, in, env, stack);
    out.emplace_back(pos, *this, *args, *body);
  }
}
