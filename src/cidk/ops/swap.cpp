#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/swap.hpp"
#include "cidk/read.hpp"
#include "cidk/types/reg.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  const SwapType Swap("swap");

  SwapType::SwapType(const string &id): OpType(id) {}

  void SwapType::init(Cx &cx, Op &op, const Val &x, const Val &y) const {
    op.args[0] = x;
    op.args[1] = y;
  }

  void SwapType::compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);
    for (int i(0); i < 2; i++) { args[i].compile(p, env, opts); }
    
    if (auto &x(args[0]); x.type != &cx.int_type && x.type != &cx.reg_type) {
      throw ESys(p, "Invalid swap place: ", x);
    }

    if (auto &y(args[1]); y.type != &cx.int_type && y.type != &cx.reg_type) {
      throw ESys(p, "Invalid swap place: ", y);
    }
    
    out.push_back(*in);
  }
    
  void SwapType::mark_refs(Op &op) const {
    auto &args(op.args);
    for (int i(0); i < 2; i++) { args[i].mark_refs(); }
  }
  
  void SwapType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    Val x(cx.int_type, Int(1)), y(cx.int_type, Int(0));

    auto v(read_val(cx, pos, in));
    if (!v) { throw ESys(p, "Missing ;"); }

    if (!v->is_eop()) {
      x = *v;
      v = read_val(cx, pos, in);
      if (!v) { throw ESys(p, "Missing ;"); }

      if (!v->is_eop()) {
        y = *v;
        read_eop(pos, in);
      }
    }
    
    out.emplace_back(cx, p, *this, x, y);
  }
}
