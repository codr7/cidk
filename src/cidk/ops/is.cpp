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
                       Stack &stack,
                       Ops &out,
                       Opts &opts) const {
    auto &d(in->as<IsData>());
    d.x.compile(cx, in->pos, env, stack, opts);
    d.y.compile(cx, in->pos, env, stack, opts);
    out.push_back(*in);
  }

  void IsType::eval(Cx &cx, Op &op, Env &env, Regs &regs, Stack &stack) const {
    auto &p(op.pos);
    auto &d(op.as<IsData>());
    d.x.push(cx, p, env, regs, stack);
    d.y.push(cx, p, env, regs, stack);
    stack.emplace_back(cx.bool_type, pop(p, stack).is(pop(p, stack)));
  }

  void IsType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<IsData>());
    d.x.get_ids(out);
    d.y.get_ids(out);
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
