#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/eq.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::ops {
  const EqType Eq("eq");

  EqType::EqType(const string &id): OpType(id) {}

  void EqType::init(Cx &cx, Op &op, const Val &x, const Val &y) const {
    op.args[0] = x;
    op.args[1] = y;
  }

  void EqType::compile(Cx &cx,
                       OpIter &in,
                       const OpIter &end,
                       Env &env,
                       Ops &out,
                       Opts &opts) const {
    auto &args(in->args);
    for (int i(0); i < 2; i++) { args[i].compile(in->pos, env, opts); }
    out.push_back(*in);
  }

  void EqType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);
    for (int i(0); i < 2; i++) { args[i].eval(p, env, regs); }
    auto &y(cx.pop(p)), &x(cx.peek(p));
    x.reset(cx.bool_type, x.eq(p, y));
  }

  void EqType::mark_refs(Op &op) const {
    auto &args(op.args);
    for (int i(0); i < 2; i++) { args[i].mark_refs(); }
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
