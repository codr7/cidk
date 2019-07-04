#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/do_stack.hpp"
#include "cidk/read.hpp"
#include "cidk/types/nil.hpp"

namespace cidk::ops {
  struct Data {
    Val in, body;
    Data(Val in, Val body): in(in), body(body) { }
  };
  
  const DoStackType DoStack("do-stack");

  DoStackType::DoStackType(string id): OpType(id) { }

  void DoStackType::init(Op &op, const Val &in, const Val &body) const {
    op.data = Data(in, body);
  }
  
  void DoStackType::eval(const Op &op, Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    Pos p(op.pos);
    Data d(op.as<Data>());
    Stack ds;

    if (d.in.type != &cx.nil_type) {
      d.in.eval(op.pos, env, stack);
      auto in(*pop(op.pos, stack, false));
      in.splat(p, env, ds);
    }

    d.body.eval(op.pos, env, ds);
    copy(ds.begin(), ds.end(), back_inserter(stack));
  }

  void DoStackType::read(Cx &cx, Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    Pos p(pos);
    auto _in(read_val(pos, in, env, stack));
    if (!_in) { throw ESys(p, "Missing do-stack input"); }

    auto body(read_val(pos, in, *env.cx.env_pool.get(env), stack));
    if (!body) { throw ESys(p, "Missing do-stack body"); }
    read_eop(pos, in, env, stack);
    
    out.emplace_back(p, *this, *_in, *body);
  }
}
