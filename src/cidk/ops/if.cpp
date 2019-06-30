#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/if.hpp"
#include "cidk/stack.hpp"
#include "cidk/types/nil.hpp"

namespace cidk::ops {
  struct IfData {
    Val cond, x, y;
  };

  const IfType If("if");

  IfType::IfType(string id): OpType(id) { }
  
  void IfType::init(Op &op, const Val &cond, const Val &x, const Val &y) const {
    IfData d;
    d.cond = cond;
    d.x = x;
    d.y = y;
    op.data = d;
  }

  void IfType::eval(const Op &op, Env &env) const {
    Cx &cx(env.cx);
    Stack &s(cx.stack);
    IfData d(op.as<IfData>());
    auto p(op.pos);
    Val cond((d.cond.type == &cx.nil_type) ? *pop(p, s, false) : d.cond);
    cond.eval(env);
    if (pop(p, s, false)->Bool(p)) { d.x.eval(env); }
    else { d.y.eval(env); }
  }

  void IfType::read(Cx &cx, const Pos &pos, Reader &in, Ops &out) const {
    auto p(pos);

    auto cond(in.read_val());
    if (!cond) { throw ReadE(p, "Missing if cond"); }

    auto x(in.read_val());
    if (!x) { throw ReadE(p, "Missing if branch"); }

    auto y(in.read_val());
    if (!y) { throw ReadE(p, "Missing else branch"); }

    out.emplace_back(p, *this, *cond, *x, *y);
  }
}
