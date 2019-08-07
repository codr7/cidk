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
                            Opts &opts) const {
    auto &p(in->pos);
    auto &d(in->as<DoStackData>());
    Regs regs;

    d.in.compile(cx, p, env, stack, opts);
    Stack ds;

    if (d.in.type != &cx.nil_type) {
      d.in.push(cx, p, env, regs, stack);
      pop(p, stack).splat(p, env, ds);
    }

    d.body.compile(cx, p, env, ds, opts);
    out.push_back(*in);
  }

  void DoStackType::eval(Cx &cx, Op &op, Env &env, Regs &regs, Stack &stack) const {
    auto &p(op.pos);
    auto &d(op.as<DoStackData>());
    Stack ds;
    
    if (d.in.type != &cx.nil_type) { d.in.splat(p, env, ds); }
    d.body.push(cx, p, env, regs, ds);
    copy(ds.begin(), ds.end(), back_inserter(stack));
  }

  void DoStackType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<DoStackData>());
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
