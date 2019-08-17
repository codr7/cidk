#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const PushType Push("push");

  PushType::PushType(const string &id): OpType(id) {}

  void PushType::init(Cx &cx, Op &op, const Val &val) const { op.args[0] = val; }

  void PushType::compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const {
    in->args[0].compile(in->pos, env, opts);
    out.push_back(*in);
  }

  void PushType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    op.args[0].eval(op.pos, env, regs);
  }

  void PushType::mark_refs(Op &op) const { op.args[0].mark_refs(); }

  void PushType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto v(read_val(cx, pos, in));
    if (!v || v->is_eop()) { throw ESys(p, "Missing value"); }
    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *v);
  }
}
