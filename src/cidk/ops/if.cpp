#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/if.hpp"
#include "cidk/read.hpp"
#include "cidk/stack.hpp"
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
                       Stack &stack,
                       Ops &out,
                       Opts *opts) const {
    auto &d(in->as<IfData>());
    d.cond.compile(cx, in->pos, env, stack, opts);
    d.x.compile(cx, in->pos, env, stack, opts);
    d.y.compile(cx, in->pos, env, stack, opts);
    out.push_back(*in);
  }

  void IfType::eval(Cx &cx, Op &op, Env &env, Stack &stack) const {
    auto &p(op.pos);
    auto &d(op.as<IfData>());
    d.cond.push(cx, p, env, stack);
      
    if (pop(p, stack).get_bool()) {
      if (d.x.type != &cx.nil_type) { d.x.push(cx, p, env, stack); }
    } else if (d.y.type != &cx.nil_type) {
      d.y.push(cx, p, env, stack);
    }
  }

  void IfType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<IfData>());
    d.cond.get_ids(out);
    d.x.get_ids(out);
    d.y.get_ids(out);
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
