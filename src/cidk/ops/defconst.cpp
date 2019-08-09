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
                             OpIter &in,
                             const OpIter &end,
                             Env &env,
                             Ops &out,
                             Opts &opts) const {
    auto &p(in->pos);
    auto &d(in->as<DefconstData>());
    d.val.compile(p, env, opts);
    env.add_const(cx, p, d.id, d.val);
  }

  void DefconstType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;;) {
      auto id(read_val(cx, pos, in));
      if (!id) { throw ESys(p, "Missing ;"); }
      if (id->is_eop()) { break; }

      if (id->type != &cx.sym_type) {
        throw ESys(p, "Invalid const id: ", id->type->id);
      }

      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing const value"); }
      out.emplace_back(cx, p, *this, id->as_sym, *v);
      n++;
    }
  }
}
