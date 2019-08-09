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

  void EqType::init(Cx &cx, Op &op, const Val &x, const Val &y) const {
    op.data = EqData(x, y);
  }

  void EqType::compile(Cx &cx,
                       OpIter &in,
                       const OpIter &end,
                       Env &env,
                       Ops &out,
                       Opts &opts) const {
    auto &d(in->as<EqData>());
    d.x.compile(in->pos, env, opts);
    d.y.compile(in->pos, env, opts);
    out.push_back(*in);
  }

  void EqType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &d(op.as<EqData>());
    d.x.eval(p, env, regs);
    d.y.eval(p, env, regs);
    auto &y(cx.pop(p)), &x(cx.peek(p));
    x.reset(cx.bool_type, x.eq(p, y));
  }

  void EqType::mark_refs(Op &op) const {
    auto &d(op.as<EqData>());
    d.x.mark_refs();
    d.y.mark_refs();
  }

  void EqType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto x(read_val(cx, pos, in));
    if (!x) { throw ESys(p, "Missing ;"); }

    if (x->is_eop()) {
      out.emplace_back(cx, p, *this, cx.$, cx.$);
      return;
    }

    int n(0);
    
    for (;; n++) {
      auto y(read_val(cx, pos, in));
      if (!y) { throw ESys(p, "Missing ;"); }
      if (y->is_eop()) { break; }
      out.emplace_back(cx, p, *this, *x, *y);
    }

    if (!n) { out.emplace_back(cx, p, *this, *x, cx.$); }
  }
}
