#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/dec.hpp"
#include "cidk/read.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/types/reg.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  struct DecData {
    Val n, delta;
    DecData(const Val &n, const Val &delta): n(n), delta(delta) {}
  };
  
  const DecType Dec("dec");

  DecType::DecType(const string &id): OpType(id) {}

  void DecType::init(Cx &cx, Op &op, const Val &n, const Val &delta) const {
    op.data = DecData(n, delta);
  }

  void DecType::compile(Cx &cx,
                        OpIter &in,
                        const OpIter &end,
                        Env &env,
                        Ops &out,
                        Opts &opts) const {
    auto &d(in->as<DecData>());
    d.n.compile(cx, in->pos, env, opts);
    d.delta.compile(cx, in->pos, env, opts);
    out.push_back(*in);
  }

  void DecType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &d(op.as<DecData>());

    if (d.n.type == &cx.reg_type) {
      d.delta.eval(cx, p, env, regs);
      auto &delta(cx.peek(p));
      
      if (delta.type != &cx.int_type) {
        throw ESys(p, "Expected Int, was: ", delta.type->id);
      }

      auto &n(regs[d.n.as_reg].as_int);
      n -= delta.as_int;
      delta.as_int = n;
    } else {
      d.n.eval(cx, p, env, regs); 
      d.delta.eval(cx, p, env, regs);
      auto &delta(cx.pop(p));
      
      if (delta.type != &cx.int_type) {
        throw ESys(p, "Expected Int, was: ", delta.type->id);
      }

      cx.peek(p).as_int -= delta.as_int;
    }
  }

  void DecType::mark_refs(Op &op) const {
    auto &d(op.as<DecData>());
    d.n.mark_refs();
    d.delta.mark_refs();
  }

  void DecType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto n(read_val(cx, pos, in));
    if (!n) { throw ESys(p, "Missing ;"); }
    Val one(cx.int_type, Int(1));
    
    if (n->is_eop()) {
      out.emplace_back(cx, p, *this, cx.$, one);
    } else {
      auto delta(read_val(cx, pos, in));
      if (!delta) { throw ESys(p, "Missing ;"); }
      
      if (delta->is_eop()) { out.emplace_back(cx, p, *this, *n, one); }
      else {
        out.emplace_back(cx, p, *this, *n, *delta);
        read_eop(pos, in);
      }
    }
  }
}
