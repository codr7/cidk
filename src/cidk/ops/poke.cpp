#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/poke.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/nil.hpp"

namespace cidk::ops {
  const PokeType Poke("poke");

  PokeType::PokeType(const string &id): OpType(id) {}

  void PokeType::init(Cx &cx, Op &op, const Val &offs, const Val &val) const {
    op.args[0] = offs;
    op.args[1] = val;
  }

  void PokeType::compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);
    for (int i(0); i < 2; i++) { args[i].compile(in->pos, env, opts); }

    auto &offs(args[0]);
    if (offs.type != &cx.int_type) { throw ESys(p, "Expected Int: ", offs); }
    out.push_back(*in);
  }

  void PokeType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);
    auto offs(args[0].as_int);
    auto &val(args[1]);
    bool is_expr(val.type == &cx.expr_type);
    auto ss(cx.stackp - cx.stack.begin());
    
    if (is_expr && offs > 1) { cx.push(p, cx.stack[ss - offs]); }
    val.eval(p, env, regs);
    swap(cx.peek(p), cx.stack[ss - offs]);
    if (!is_expr || offs > 1) { cx.pop(p); }
  }

  void PokeType::mark_refs(Op &op) const { op.args[1].mark_refs(); }

  void PokeType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    vector<Val> vals;
    Val offs(cx.int_type, Int(0));
    
    for (;; offs.as_int++) {
      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      vals.push_back(*v);
    }

    if (!offs.as_int) { throw ESys(p, "Missing poke value"); }
    
    for (auto &v: vals) {
      if (v.type != &cx.nil_type) { out.emplace_back(cx, p, *this, offs, v); }
      offs.as_int--;
    }
  }
}
