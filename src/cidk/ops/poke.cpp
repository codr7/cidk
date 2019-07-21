#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/poke.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/nil.hpp"

namespace cidk::ops {
  struct PokeData {
    size_t offs;
    Val val;
    bool is_expr;
    
    PokeData(size_t offs, const Val &val):
      offs(offs), val(val), is_expr(val.type == &val.type->cx.expr_type) {}
  };
  
  const PokeType Poke("poke");

  PokeType::PokeType(const string &id): OpType(id) {}

  void PokeType::init(Cx &cx, Op &op, size_t offs, const Val &val) const {
    op.data = PokeData(offs, val);
  }

  void PokeType::compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Stack &stack,
                         Ops &out,
                         Opts *opts) const {
    in->as<PokeData>().val.compile(cx, in->pos, env, stack, opts);
    out.push_back(*in);
  }

  void PokeType::eval(Cx &cx, Op &op, Env &env, Stack &stack) const {
    const Pos &p(op.pos);
    auto &d(op.as<PokeData>());
    auto ss(stack.size());
    if (d.is_expr && d.offs > 1) { stack.push_back(stack[ss - d.offs]); }
    d.val.eval(cx, p, env, stack);
    swap(stack.back(), stack[ss - d.offs]);
    if (!d.is_expr || d.offs > 1) { stack.pop_back(); }
  }

  void PokeType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<PokeData>());
    d.val.get_ids(out);
  }

  void PokeType::mark_refs(Op &op) const {
    auto &d(op.as<PokeData>());
    d.val.mark_refs();
  }

  void PokeType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    vector<Val> vals;
    size_t n(0);
    
    for (;; n++) {
      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      vals.push_back(*v);
    }

    if (!n) { throw ESys(p, "Missing poke value"); }
    
    for (auto &v: vals) {
      if (v.type != &cx.nil_type) { out.emplace_back(cx, p, *this, n, v); }
      n--;
    }
  }
}
