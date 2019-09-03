#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/if.hpp"
#include "cidk/read.hpp"
#include "cidk/types/nil.hpp"

namespace cidk::ops {
  const IfType If("if");

  IfType::IfType(const string &id): OpType(id) {}
  
  void IfType::init(Cx &cx,
                    Op &op,
                    const Val &cond,
                    const Val &x,
                    const Val &y) const {
    op.args[0] = cond;
    op.args[1] = x;
    op.args[2] = y;
  }

  void IfType::compile(Cx &cx,
                       OpIter &in,
                       const OpIter &end,
                       Env &env,
                       Ops &out,
                       Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);
    for (int i(0); i < 3; i++) { args[i].compile(p, env, opts); }
    out.push_back(*in);
  }

  bool IfType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);
    args[0].eval(p, env, regs);
    auto &x(args[1]), &y(args[2]);
    
    if (cx.pop(p).get_bool()) {
      if (x.type != &cx.nil_type && !x.eval(p, env, regs)) { return false; }
    } else if (y.type != &cx.nil_type && !y.eval(p, env, regs)) {
      return false;
    }

    return true;
  }

  bool IfType::find_op(Op &op, function<bool (Ops &, OpIter &)> pred) const {
    auto &args(op.args);

    for (int i(0); i < 3; i++) {
      if (args[i].find_op(pred)) { return true; }
    }

    return false;
  }

  void IfType::mark_refs(Op &op) const {
    auto &args(op.args);
    for (int i(0); i < 3; i++) { args[i].mark_refs(); }
  }

  void IfType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);

    auto cond(read_val(cx, pos, in));
    if (!cond || cond->is_eop()) { throw ESys(p, "Missing if cond"); }

    auto x(read_val(cx, pos, in));
    if (!x  || x->is_eop()) { throw ESys(p, "Missing if branch"); }

    auto y(read_val(cx, pos, in));
    if (!y || y->is_eop()) { throw ESys(p, "Missing else branch"); }
    read_eop(pos, in);

    out.emplace_back(cx, p, *this, *cond, *x, *y);
  }
}
