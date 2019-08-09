#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/if.hpp"
#include "cidk/read.hpp"
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

  void IfType::compile(Cx &cx,
                       OpIter &in,
                       const OpIter &end,
                       Env &env,
                       Ops &out,
                       Opts &opts) const {
    auto &p(in->pos);
    auto &d(in->as<IfData>());
    d.cond.compile(p, env, opts);
    d.x.compile(p, env, opts);
    d.y.compile(p, env, opts);
    out.push_back(*in);
  }

  void IfType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &d(op.as<IfData>());
    d.cond.eval(p, env, regs);
      
    if (cx.pop(p).get_bool()) {
      if (d.x.type != &cx.nil_type) { d.x.eval(p, env, regs); }
    } else if (d.y.type != &cx.nil_type) {
      d.y.eval(p, env, regs);
    }
  }

  void IfType::mark_refs(Op &op) const {
    auto &d(op.as<IfData>());
    d.cond.mark_refs();
    d.x.mark_refs();
    d.y.mark_refs();
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
