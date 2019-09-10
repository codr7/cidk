#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/is.hpp"
#include "cidk/read.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  const IsType Is("is");

  IsType::IsType(const string &id): OpType(id) {}

  void IsType::init(Cx &cx, Op &op, const Val &x, const Val &y) const {
    op.args[0] = x;
    op.args[1] = y;
  }

  void IsType::compile(Cx &cx, Op &op, Env &env, Ops &out, Opts &opts) const {
    auto &p(op.pos);
    auto &args(op.args);
    for (int i(0); i < 2; i++) { args[i].compile(p, env, opts); }
    out.push_back(op);
  }

  bool IsType::find_op(Op &op, function<bool (Ops &, OpIter &)> pred) const {
    auto &args(op.args);

    for (int i(0); i < 2; i++) {
      if (args[i].find_op(pred)) { return true; }
    }

    return false;
  }

  void IsType::mark_refs(Op &op) const {
    auto &args(op.args);
    for (int i(0); i < 2; i++) { args[i].mark_refs(); }
  }

  void IsType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto x(read_val(cx, pos, in));
    if (!x) { throw ESys(p, "Missing ;"); }
    optional<Val> y(cx.$);

    if (x->is_eop()) {
      x = cx.$;
    } else {
      y = read_val(cx, pos, in);
      if (!y) { throw ESys(p, "Missing ;"); }
      if (y->is_eop()) {
        y = cx.$;
      } else {
        read_eop(pos, in);
      }
    }
    
    out.emplace_back(cx, p, *this, *x, *y);
  }
}
