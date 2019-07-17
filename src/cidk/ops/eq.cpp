#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/eq.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::ops {
  struct EqData {
    Val x, y;
    EqData(const Val &x, const Val &y): x(x), y(y) {}
  };
  
  const EqType Eq("eq");

  EqType::EqType(const string &id): OpType(id) {}

  void EqType::init(Op &op, const Val &x, const Val &y) const {
    op.data = EqData(x, y);
  }

  void EqType::eval(const Op &op, Env &env, Stack &stack) const {
    const Pos &p(op.pos);
    const EqData &d(op.as<EqData>());
    d.x.eval(p, env, stack);
    auto x(pop(p, stack));
    d.y.eval(p, env, stack);
    auto y(pop(p, stack));
    stack.emplace_back(op.pos, env.cx.bool_type, x.eq(p, y));
  }

  void EqType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<EqData>());
    d.x.get_ids(out);
    d.y.get_ids(out);
  }

  void EqType::mark_refs(Op &op) const {
    auto &d(op.as<EqData>());
    d.x.mark_refs();
    d.y.mark_refs();
  }

  void EqType::read(Cx &cx,
                    Pos &pos,
                    istream &in,
                    ReadState &state,
                    Env &env,
                    Stack &stack,
                    Ops &out) const {
    Pos p(pos);
    auto x(read_val(pos, in, state, env, stack));
    if (!x) { throw ESys(p, "Missing ;"); }

    if (x->is_eop()) {
      out.emplace_back(p, *this, cx.S, cx.S);
      return;
    }

    int n(0);
    
    for (;; n++) {
      auto y(read_val(pos, in, state, env, stack));
      if (!y) { throw ESys(p, "Missing ;"); }
      if (y->is_eop()) { break; }
      out.emplace_back(p, *this, *x, *y);
    }

    if (!n) { out.emplace_back(p, *this, *x, cx.S); }
  }
}
