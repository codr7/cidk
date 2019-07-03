#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/if.hpp"
#include "cidk/read.hpp"
#include "cidk/stack.hpp"
#include "cidk/types/nil.hpp"

namespace cidk::ops {
  struct Data {
    Val cond, x, y;
  };

  const IfType If("if");

  IfType::IfType(string id): OpType(id) { }
  
  void IfType::init(Op &op, const Val &cond, const Val &x, const Val &y) const {
    Data d;
    d.cond = cond;
    d.x = x;
    d.y = y;
    op.data = d;
  }

  void IfType::eval(const Op &op, Env &env) const {
    Cx &cx(env.cx);
    Stack &s(cx.stack);
    Data d(op.as<Data>());
    auto p(op.pos);
    d.cond.eval(p, env);
    if (pop(p, s, false)->Bool(p)) { d.x.eval(p, env); }
    else { d.y.eval(p, env); }
  }

  void IfType::read(Cx &cx, Pos &pos, istream &in, Env &env, Ops &out) const {
    auto p(pos);

    auto cond(read_val(pos, in, env));
    if (!cond) { throw ESys(p, "Missing if cond"); }

    auto x(read_val(pos, in, env));
    if (!x) { throw ESys(p, "Missing if branch"); }

    auto y(read_val(pos, in, env));
    if (!y) { throw ESys(p, "Missing else branch"); }
    read_eop(pos, in, env);

    out.emplace_back(p, *this, *cond, *x, *y);
  }
}
