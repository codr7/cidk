#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/drop.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const DropType Drop("drop");

  DropType::DropType(const string &id): OpType(id) {}

  void DropType::init(Cx &cx, Op &op, const Val &n) const {
    op.args[0] = n;
  }

  void DropType::compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const {
    auto &p(in->pos);
    auto &n(in->args[0]);
    n.compile(in->pos, env, opts);
    if (n.type != &cx.int_type) { throw ESys(p, "Expected Int: ", n.type->id); }
    out.push_back(*in);
  }

  void DropType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto n(op.args[0].as_int);
    if (n > cx.stackp - cx.stack.begin()) { throw ESys(p, "Nothing to drop"); }
    cx.stackp -= n;
  }

  void DropType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos vp(pos);
    auto v(read_val(cx, pos, in));
    if (!v) { throw ESys(pos, "Missing ;"); }
    
    Int n(1);

    if (!v->is_eop()) {
      n = v->as_int;
      read_eop(pos, in);
    }

    out.emplace_back(cx, pos, *this, Val(cx.int_type, n));
  }
}
