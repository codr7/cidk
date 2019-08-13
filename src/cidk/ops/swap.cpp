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
    auto &args(in->args);
    for (int i(0); i < 2; i++) { args[i].compile(in->pos, env, opts); }
    out.push_back(*in);
  }
  
  static Val &get_ref(Cx &cx,
                      const Pos &pos,
                      Val &place,
                      Env &env,
                      Reg *regs) {
    if (place.type == &cx.int_type) {
      return cx.stack[cx.stackp - cx.stack.begin() - place.as_int - 1];
    }

    return (place.type == &cx.reg_type)
      ? regs[place.as_reg]
      : env.get(pos, place.as_sym);
  }
  
  void SwapType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);

    swap(get_ref(cx, p, args[0], env, regs),
         get_ref(cx, p, args[1], env, regs));
  }

  void SwapType::mark_refs(Op &op) const {
    auto &args(op.args);
    for (int i(0); i < 2; i++) { args[i].mark_refs(); }
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
