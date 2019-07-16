#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/let.hpp"
#include "cidk/read.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  struct LetData {
    const Sym *key;
    Val val;
    
    LetData(const Sym *key, const Val &val): key(key), val(val) {}
  };

  const LetType Let("let");

  LetType::LetType(const string &id): OpType(id) {}

  void LetType::init(Op &op, const Sym *key, const Val &val) const {
    op.data = LetData(key, val);
  }

  void LetType::eval(const Op &op, Env &env, Stack &stack) const {
    const Pos &p(op.pos);
    const LetData &d(op.as<LetData>());
    Val v;
    d.val.clone(p, v);
    v.eval(p, env, stack);
    env.set(p, d.key, pop(p, stack), false);
  }

  void LetType::get_ids(const Op &op, IdSet &out) const {
    op.as<LetData>().val.get_ids(out);
  }

  void LetType::mark_refs(Op &op) const { op.as<LetData>().val.mark_refs(); }

  void LetType::read(Cx &cx,
                     Pos &pos,
                     istream &in,
                     ReadState &state,
                     Env &env,
                     Stack &stack,
                     Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;;) {
      auto k(read_val(pos, in, state, env, stack));
      if (!k) { throw ESys(p, "Missing ;"); }
      if (k->is_eop()) { break; }

      if (k->type != &cx.sym_type) {
        throw ESys(p, "Invalid let id: ", k->type->id);
      }

      auto v(read_val(pos, in, state, env, stack));
      if (!v) { throw ESys(p, "Missing let value"); }
      out.emplace_back(p, *this, k->as_sym, *v);
      n++;
    }

    if (!n) {
      auto v(pop(p, stack));
      out.emplace_back(p, *this, pop(p, stack).as_sym, v);
    }

    if (!state.env_depth) { state.env_extend = true; }
  }
}
