#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/defconst.hpp"
#include "cidk/read.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  const DefconstType Defconst("defconst");

  DefconstType::DefconstType(const string &id): OpType(id) {}

  void DefconstType::init(Cx &cx, Op &op, const Val &id, const Val &val) const {
    op.args[0] = id;
    op.args[1] = val;
  }

  void DefconstType::compile(Cx &cx,
                             OpIter &in,
                             const OpIter &end,
                             Env &env,
                             Ops &out,
                             Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);
    
    auto &id(args[0]);
    if (id.type != &cx.sym_type) { throw ESys(p, "Expected id: ", id.type->id); }

    auto &v(args[1]);
    v.compile(p, env, opts);    
    env.add_const(p, id.as_sym, v);
  }

  void DefconstType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto id(read_val(cx, pos, in));
    if (!id || id->is_eop()) { throw ESys(p, "Missing id"); }

    auto v(read_val(cx, pos, in));
    if (!v || v->is_eop()) { throw ESys(p, "Missing value"); }
    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *id, *v);
  }
}
