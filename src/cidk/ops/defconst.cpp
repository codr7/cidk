#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/defconst.hpp"
#include "cidk/read.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  struct DefconstData {
    const Sym *id;
    Val val;
    DefconstData(const Sym *id, const Val &val): id(id), val(val) {}
  };
    
  const DefconstType Defconst("defconst");

  DefconstType::DefconstType(const string &id): OpType(id) {}

  void DefconstType::init(Cx &cx, Op &op, const Sym *id, const Val &val) const {
    op.data = DefconstData(id, val);
  }

  void DefconstType::compile(Cx &cx,
                             Op &op,
                             Env &env,
                             Stack &stack,
                             Ops &out,
                             Opts *opts) const {
    auto &d(op.as<DefconstData>());
    d.val.compile(cx, op.pos, env, stack, opts);
    env.add_const(op.pos, d.id, d.val);
  }

  void DefconstType::read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;;) {
      auto id(read_val(pos, in, env, stack));
      if (!id) { throw ESys(p, "Missing ;"); }
      if (id->is_eop()) { break; }

      if (id->type != &cx.sym_type) {
        throw ESys(p, "Invalid const id: ", id->type->id);
      }

      auto v(read_val(pos, in, env, stack));
      if (!v) { throw ESys(p, "Missing const value"); }
      out.emplace_back(cx, p, *this, id->as_sym, *v);
      n++;
    }

    if (!n) {
      auto v(pop(p, stack));
      out.emplace_back(cx, p, *this, pop(p, stack).as_sym, v);
    }
  }
}
