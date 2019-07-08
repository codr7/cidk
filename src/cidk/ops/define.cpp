#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/define.hpp"
#include "cidk/read.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  const DefineType Define("define");

  DefineType::DefineType(string id): OpType(id) { }

  void DefineType::eval(const Op &op, Env &env, Stack &stack) const { }

  void DefineType::read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;;) {
      auto k(read_val(pos, in, env, stack));
      if (!k) { throw ESys(p, "Missing ;"); }
      if (k->is_eop()) { break; }

      if (k->type != &cx.sym_type) {
        throw ESys(p, "Invalid const id: ", k->type->id);
      }

      auto v(read_val(pos, in, env, stack));
      if (!v) { throw ESys(p, "Missing const value"); }
      env.add_const(p, k->as_sym, *v);
      n++;
    }

    if (!n) {
      auto v(*pop(p, stack, false)), k(*pop(p, stack, false));
      env.add_const(p, k.as_sym, v);      
    }
  }
}
