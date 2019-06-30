#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/if.hpp"
#include "cidk/stack.hpp"

namespace cidk::ops {
  struct IfData {
    Val x, y;
  };

  const IfType If("if");

  IfType::IfType(string id): OpType(id) { }
  
  void IfType::init(Op &op, const Val &x, const Val &y) const {
    IfData d;
    d.x = x;
    d.y = y;
    op.data = d;
  }

  void IfType::eval(const Op &op, Env &env) const {
    IfData d(op.as<IfData>());
    auto p(op.pos);
    auto cond(pop(p, env.cx.stack, false));
    if (cond->_bool(p)) { d.x.eval(env); }
    else { d.y.eval(env); }
  }

  void IfType::read(Cx &cx, const Pos &pos, Reader &in, Ops &out) const {
    auto p(pos);

    auto x(in.read_val());
    if (!x) { throw ReadE(p, "Missing if"); }

    auto y(in.read_val());
    if (!y) { throw ReadE(p, "Missing else"); }

    out.emplace_back(p, *this, *x, *y);
  }
}
