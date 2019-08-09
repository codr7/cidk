#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/do_stack.hpp"
#include "cidk/read.hpp"
#include "cidk/types/nil.hpp"

namespace cidk::ops {
  struct DoStackData {
    Val body;
    DoStackData(const Val &body): body(body) {}
  };
  
  const DoStackType DoStack("do-stack");

  DoStackType::DoStackType(const string &id): OpType(id) {}

  void DoStackType::init(Cx &cx, Op &op, const Val &body) const {
    op.data = DoStackData(body);
  }

  void DoStackType::compile(Cx &cx,
                            OpIter &in,
                            const OpIter &end,
                            Env &env,
                            Ops &out,
                            Opts &opts) const {
    auto &p(in->pos);
    auto &d(in->as<DoStackData>());

    vector<Val> s;    
    copy(cx.stack.begin(), cx.stackp, back_inserter(s));
    cx.stackp = &cx.stack[0];
    
    d.body.compile(cx, p, env, opts);

    copy(cx.stack.begin(), cx.stackp, back_inserter(s));
    move(s.begin(), s.end(), cx.stack.begin());
    cx.stackp = &cx.stack[s.size()];
    
    out.push_back(*in);
  }

  void DoStackType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &d(op.as<DoStackData>());
    
    vector<Val> s;    
    copy(cx.stack.begin(), cx.stackp, back_inserter(s));
    cx.stackp = &cx.stack[0];

    d.body.eval(cx, p, env, regs);

    copy(cx.stack.begin(), cx.stackp, back_inserter(s));
    move(s.begin(), s.end(), cx.stack.begin());
    cx.stackp = &cx.stack[s.size()];
  }

  void DoStackType::mark_refs(Op &op) const {
    auto &d(op.as<DoStackData>());
    d.body.mark_refs();
  }

  void DoStackType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);

    auto body(read_val(cx, pos, in));
    if (!body) { throw ESys(p, "Missing body"); }
    read_eop(pos, in);
    
    out.emplace_back(cx, p, *this, *body);
  }
}
