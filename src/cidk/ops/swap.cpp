#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/swap.hpp"
#include "cidk/read.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  struct SwapData {
    Val x, y;
    SwapData(const Val &x, const Val &y): x(x), y(y) {}
  };

  const SwapType Swap("swap");

  SwapType::SwapType(const string &id): OpType(id) {}

  void SwapType::init(Cx &cx, Op &op, const Val &x, const Val &y) const {
    op.data = SwapData(x, y);
  }

  void SwapType::eval(Cx &cx, Op &op, Env &env, Stack &stack) const {
    const Pos &p(op.pos);
    const auto &d(op.as<SwapData>());
    const auto ss(stack.size());
    
    Val
      &x((d.x.type == &cx.int_type)
         ? stack[ss - d.x.as_int - 1]
         : env.get(p, d.x.as_sym)),
      &y((d.y.type == &cx.int_type)
         ? stack[ss - d.y.as_int - 1]
         : env.get(p, d.y.as_sym));

    swap(x, y);
  }

  void SwapType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;;) {
      auto x(read_val(cx, pos, in));
      if (!x) { throw ESys(p, "Missing ;"); }
      if (x->is_eop()) { break; }

      if (x->type != &cx.int_type && x->type != &cx.sym_type) {
        throw ESys(p, "Invalid swap place", x->type->id);
      }

      n++;
      auto y(read_val(cx, pos, in));
      if (!y) { throw ESys(p, "Missing ;"); }

      if (y->is_eop()) {
        out.emplace_back(cx, p, *this, *x, Val(cx.int_type, Int(0)));
        break;
      }

      if (y->type != &cx.int_type && y->type != &cx.sym_type) {
        throw ESys(p, "Invalid swap place", y->type->id);
      }

      out.emplace_back(cx, p, *this, *x, *y);
    }

    if (!n) {
      out.emplace_back(cx,
                       pos,
                       *this,
                       Val(cx.int_type, Int(1)),
                       Val(cx.int_type, Int(0)));
    }
  }
}
