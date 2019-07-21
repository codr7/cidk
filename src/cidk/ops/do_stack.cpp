#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/do_stack.hpp"
#include "cidk/read.hpp"
#include "cidk/types/nil.hpp"

namespace cidk::ops {
  struct DoStackData {
    Val in, body;
    DoStackData(const Val &in, const Val &body): in(in), body(body) {}
  };
  
  const DoStackType DoStack("do-stack");

  DoStackType::DoStackType(const string &id): OpType(id) {}

  void DoStackType::init(Cx &cx, Op &op, const Val &in, const Val &body) const {
    op.data = DoStackData(in, body);
  }

  void DoStackType::compile(Cx &cx,
                            OpIter &in,
                            const OpIter &end,
                            Env &env,
                            Stack &stack,
                            Ops &out,
                            Opts *opts) const {
    const Pos &p(in->pos);
    auto &d(in->as<DoStackData>());
    d.in.compile(cx, p, env, stack, opts);

    Stack ds;

    if (d.in.type != &cx.nil_type) {
      d.in.eval(cx, p, env, stack);
      pop(p, stack).splat(p, env, ds);
    }

    d.body.compile(cx, p, env, ds, opts);
    out.push_back(*in);
  }

  void DoStackType::eval(Cx &cx, Op &op, Env &env, Stack &stack) const {
    const Pos &p(op.pos);
    const DoStackData &d(op.as<DoStackData>());
    Stack ds;

    if (d.in.type != &cx.nil_type) {
      d.in.eval(cx, op.pos, env, stack);
      pop(op.pos, stack).splat(p, env, ds);
    }

    d.body.eval(cx, op.pos, env, ds);
    copy(ds.begin(), ds.end(), back_inserter(stack));
  }

  void DoStackType::get_ids(const Op &op, IdSet &out) const {
    DoStackData d(op.as<DoStackData>());
    d.in.get_ids(out);
    d.body.get_ids(out);
  }

  void DoStackType::mark_refs(Op &op) const {
    auto &d(op.as<DoStackData>());
    d.in.mark_refs();
    d.body.mark_refs();
  }

  void DoStackType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto _in(read_val(cx, pos, in));
    if (!_in) { throw ESys(p, "Missing do-stack input"); }

    auto body(read_val(cx, pos, in));
    if (!body) { throw ESys(p, "Missing do-stack body"); }
    read_eop(pos, in);
    
    out.emplace_back(cx, p, *this, *_in, *body);
  }
}
