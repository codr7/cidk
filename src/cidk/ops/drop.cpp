#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/drop.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const DropType Drop("drop");

  DropType::DropType(const string &id): OpType(id) {}

  void DropType::init(Cx &cx, Op &op, Int n) const { op.data = n; }

  void DropType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto n(op.as<Int>());
    if (n > cx.stackp - cx.stack.begin()) { throw ESys(p, "Nothing to drop"); }
    cx.stackp -= n;
  }

  void DropType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos vp(pos);
    auto v(read_val(cx, pos, in));
    if (!v) { throw ESys(pos, "Missing ;"); }
    Int n(1);

    if (!v->is_eop()) {
      if (v->type != &cx.int_type) {
        throw ESys(vp, "Invalid drop n, expected Int: ", v->type->id);
      }

      n = v->as_int;
      read_eop(pos, in);
    }

    out.emplace_back(cx, pos, *this, n);
  }
}
