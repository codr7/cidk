#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/expr.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/ops/fail.hpp"
#include "cidk/read.hpp"
#include "cidk/types/e.hpp"
#include "cidk/types/str.hpp"

namespace cidk::ops {
  const FailType Fail("fail");

  FailType::FailType(const string &id): OpType(id) {}

  void FailType::init(Cx &cx, Op &op, const Val &msg) const { op.args[0] = msg; }

  void FailType::compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const {
    auto &p(in->pos);
    auto &msg(in->args[0]);
    msg.compile(p, env, opts);
    out.push_back(*in);
  }

  void FailType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    cx.eval_state = EvalState::error;
    cx.e = cx.e_type.pool.get(cx, op.pos, op.args[0]);
  }

  bool FailType::find_op(Op &op, function<bool (Ops &, OpIter &)> pred) const {
    return op.args[0].find_op(pred);
  }

  void FailType::mark_refs(Op &op) const { op.args[0].mark_refs(); }

  void FailType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing body"); }
    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *body);
  }
}
