#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/if.hpp"
#include "cidk/read.hpp"
#include "cidk/stack.hpp"
#include "cidk/types/nil.hpp"

namespace cidk::ops {
  struct IfData {
    Val cond, x, y;
    IfData(const Val &cond, const Val &x, const Val &y): cond(cond), x(x), y(y) {}
  };

  const IfType If("if");

  IfType::IfType(const string &id): OpType(id) {}
  
  void IfType::init(Cx &cx,
                    Op &op,
                    const Val &cond,
                    const Val &x,
                    const Val &y) const {
    op.data = IfData(cond, x, y);
  }

  void IfType::eval(Op &op, Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    auto &p(op.pos);
    auto &d(op.as<IfData>());

    d.cond.eval(p, env, stack);
    
    if (pop(p, stack).get_bool()) {
      if (d.x.type != &cx.nil_type) { d.x.eval(p, env, stack); }
    } else if (d.y.type != &cx.nil_type) {
      d.y.eval(p, env, stack);
    }
  }

  void IfType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<IfData>());
    d.cond.get_ids(out);
    d.x.get_ids(out);
    d.y.get_ids(out);
  }

  void IfType::mark_refs(Op &op) const {
    auto &d(op.as<IfData>());
    d.cond.mark_refs();
    d.x.mark_refs();
    d.y.mark_refs();
  }

  void IfType::read(Cx &cx,
                    Pos &pos,
                    istream &in,
                    ReadState &state,
                    Env &env,
                    Stack &stack,
                    Ops &out) const {
    Pos p(pos);

    auto cond(read_val(pos, in, state, env, stack));
    if (!cond) { throw ESys(p, "Missing if cond"); }

    auto x(read_val(pos, in, state, env, stack));
    if (!x) { throw ESys(p, "Missing if branch"); }

    auto y(read_val(pos, in, state, env, stack));
    if (!y) { throw ESys(p, "Missing else branch"); }
    read_eop(pos, in, env, stack);

    out.emplace_back(cx, p, *this, *cond, *x, *y);
  }
}
