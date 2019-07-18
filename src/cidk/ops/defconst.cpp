#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/defconst.hpp"
#include "cidk/read.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  const DefconstType Defconst("defconst");

  DefconstType::DefconstType(const string &id): OpType(id) {}

  void DefconstType::read(Cx &cx,
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
        throw ESys(p, "Invalid const id: ", k->type->id);
      }

      auto v(read_val(pos, in, state, env, stack));
      if (!v) { throw ESys(p, "Missing const value"); }
      env.add_const(p, k->as_sym, *v);
      n++;
    }

    if (!n) {
      auto v(pop(p, stack));
      env.add_const(p, pop(p, stack).as_sym, v);      
    }
  }
}
