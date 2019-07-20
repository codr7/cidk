#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/is.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"

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
                       Op &op,
                       Env &env,
                       Stack &stack,
                       Ops &out,
                       Opts *opts) const {
    auto &d(op.as<IsData>());
    d.x.compile(cx, op.pos, env, stack, opts);
    d.y.compile(cx, op.pos, env, stack, opts);
    out.push_back(op);
  }

  void IsType::eval(Op &op, Env &env, Stack &stack) const {
    const Pos &p(op.pos);
    const IsData &d(op.as<IsData>());
    d.x.eval(p, env, stack);
    auto x(pop(p, stack));
    d.y.eval(p, env, stack);
    auto y(pop(p, stack));
    stack.emplace_back(op.pos, env.cx.bool_type, x.is(y));
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

  void IsType::read(Cx &cx,
                    Pos &pos,
                    istream &in,
                    Env &env,
                    Stack &stack,
                    Ops &out) const {
    Pos p(pos);
    auto x(read_val(pos, in, env, stack));
    if (!x) { throw ESys(p, "Missing ;"); }

    if (x->is_eop()) {
      out.emplace_back(cx, p, *this, cx.$, cx.$);
      return;
    }

    int n(0);
    
    for (;; n++) {
      auto y(read_val(pos, in, env, stack));
      if (!y) { throw ESys(p, "Missing ;"); }
      if (y->is_eop()) { break; }
      out.emplace_back(cx, p, *this, *x, *y);
    }

    if (!n) { out.emplace_back(cx, p, *this, *x, cx.$); }
  }
}
