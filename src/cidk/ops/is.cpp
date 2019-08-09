#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/is.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  struct IsData {
    Val x, y;    
    IsData(const Val &x, const Val &y): x(x), y(y) {}
  };
  
  const IsType Is("is");

  IsType::IsType(const string &id): OpType(id) {}

  void IsType::init(Cx &cx, Op &op, const Val &x, const Val &y) const {
    op.data = IsData(x, y);
  }

  void IsType::compile(Cx &cx,
                       OpIter &in,
                       const OpIter &end,
                       Env &env,
                       Ops &out,
                       Opts &opts) const {
    auto &p(in->pos);
    auto &d(in->as<IsData>());
    d.x.compile(p, env, opts);
    d.y.compile(p, env, opts);
    out.push_back(*in);
  }

  void IsType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &d(op.as<IsData>());
    d.x.eval(p, env, regs);
    d.y.eval(p, env, regs);
    auto &y(cx.pop(p)), &x(cx.peek(p));    
    x.reset(cx.bool_type, x.is(y));
  }

  void IsType::mark_refs(Op &op) const {
    auto &d(op.as<IsData>());
    d.x.mark_refs();
    d.y.mark_refs();
  }

  void IsType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
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
