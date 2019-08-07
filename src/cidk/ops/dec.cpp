#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/dec.hpp"
#include "cidk/read.hpp"
#include "cidk/types/pop.hpp"
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
                        Stack &stack,
                        Ops &out,
                        Opts &opts) const {
    auto &d(in->as<DecData>());
    d.n.compile(cx, in->pos, env, stack, opts);
    d.delta.compile(cx, in->pos, env, stack, opts);
    out.push_back(*in);
  }

  void DecType::eval(Cx &cx, Op &op, Env &env, Reg *regs, Stack &stack) const {
    auto &p(op.pos);
    auto &d(op.as<DecData>());
    d.n.push(cx, p, env, regs, stack);
    d.delta.push(cx, p, env, regs, stack);
    auto &delta(pop(p, stack));

    if (delta.type != &cx.int_type) {
      throw ESys(p, "Expected Int, was: ", delta.type->id);
    }

    auto &n(stack.back());

    if (n.type != &cx.int_type) {
      throw ESys(p, "Expected Int, was: ", n.type->id);
    }

    n.as_int -= delta.as_int;
  }

  void DecType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<DecData>());
    d.n.get_ids(out);
    d.delta.get_ids(out);
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
