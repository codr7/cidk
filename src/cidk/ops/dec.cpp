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

  void DecType::eval(Op &op, Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    const Pos &p(op.pos);
    auto &d(op.as<DecData>());
    d.delta.eval(p, env, stack);
    Val delta(pop(p, stack));

    if (delta.type != &cx.int_type) {
      throw ESys(p, "Expected Int, was: ", delta.type->id);
    }
    
    if (d.n.type == &cx.pop_type) {
      Val &n(stack.back());

      if (n.type != &cx.int_type) {
        throw ESys(p, "Expected Int, was: ", n.type->id);
      }
      
      n.as_int -= delta.as_int;
    } else if (d.n.type == &cx.int_type) {
      stack.emplace_back(p, cx.int_type, d.n.as_int - delta.as_int);
    } else if (d.n.type == &cx.sym_type) {
      Val &n(env.get_item(p, d.n.as_sym).val);
      
      if (n.type != &cx.int_type) {
        throw ESys(p, "Expected Int, was: ", n.type->id);
      }

      n.as_int -= delta.as_int;
      stack.push_back(n);
    } else {
      throw ESys(p, "Expected Int|Sym, was: ", d.n.type->id);
    }
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

  void DecType::read(Cx &cx,
                     Pos &pos,
                     istream &in,
                     ReadState &state,
                     Env &env,
                     Stack &stack,
                     Ops &out) const {
    Pos p(pos);
    auto n(read_val(pos, in, state, env, stack));
    if (!n) { throw ESys(p, "Missing ;"); }
    Val one(p, cx.int_type, Int(1));
    
    if (n->is_eop()) { out.emplace_back(cx, p, *this, cx.$, one); }
    else {
      auto delta(read_val(pos, in, state, env, stack));
      if (!delta) { throw ESys(p, "Missing ;"); }
      
      if (delta->is_eop()) { out.emplace_back(cx, p, *this, *n, one); }
      else {
        out.emplace_back(cx, p, *this, *n, *delta);
        read_eop(pos, in, env, stack);
      }
    }
  }
}
