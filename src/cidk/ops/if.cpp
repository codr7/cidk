#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/if.hpp"
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
    Val cond((d.cond.type == &cx.nil_type) ? *pop(p, s, false) : d.cond);
    cond.eval(env);
    if (pop(p, s, false)->Bool(p)) { d.x.eval(env); }
    else { d.y.eval(env); }
  }

  void IfType::read(Cx &cx, const Pos &pos, Reader &in, Env &env, Ops &out) const {
    auto p(pos);

    auto cond(in.read_val(env));
    if (!cond) { throw ERead(p, "Missing if cond"); }

    auto x(in.read_val(env));
    if (!x) { throw ERead(p, "Missing if branch"); }

    auto y(in.read_val(env));
    if (!y) { throw ERead(p, "Missing else branch"); }
    in.read_eop(env);

    out.emplace_back(p, *this, *cond, *x, *y);
  }
}
