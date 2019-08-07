#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/swap.hpp"
#include "cidk/read.hpp"
#include "cidk/types/reg.hpp"
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

  void SwapType::compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Stack &stack,
                         Ops &out,
                         Opts &opts) const {
    auto &d(in->as<SwapData>());
    d.x.compile(cx, in->pos, env, stack, opts);
    d.y.compile(cx, in->pos, env, stack, opts);
    out.push_back(*in);
  }
  
  static Val &get_ref(Cx &cx,
                      const Pos &pos,
                      Val &place,
                      Env &env,
                      Reg *regs,
                      Stack &stack) {
    if (place.type == &cx.int_type) { return stack[stack.size() - place.as_int - 1]; }
    if (place.type == &cx.reg_type) { return regs[place.as_reg].second; }
    return env.get(pos, place.as_sym);
  }
  
  void SwapType::eval(Cx &cx, Op &op, Env &env, Reg *regs, Stack &stack) const {
    auto &p(op.pos);
    auto &d(op.as<SwapData>());

    swap(get_ref(cx, p, d.x, env, regs, stack),
         get_ref(cx, p, d.y, env, regs, stack));
  }

  void SwapType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<SwapData>());
    d.x.get_ids(out);
    d.y.get_ids(out);
  }

  void SwapType::mark_refs(Op &op) const {
    auto &d(op.as<SwapData>());
    d.x.mark_refs();
    d.y.mark_refs();
  }
  
  void SwapType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;;) {
      auto x(read_val(cx, pos, in));
      if (!x) { throw ESys(p, "Missing ;"); }
      if (x->is_eop()) { break; }

      if (x->type != &cx.int_type &&
          x->type != &cx.reg_type &&
          x->type != &cx.sym_type) {
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
