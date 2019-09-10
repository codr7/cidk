#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/splat.hpp"
#include "cidk/read.hpp"
#include "cidk/types/list.hpp"

namespace cidk::ops {
  const SplatType Splat("splat");

  SplatType::SplatType(const string &id): OpType(id) {}

  void SplatType::init(Cx &cx, Op &op, const Val &val, const Val &max) const {
    op.args[0] = val;
    op.args[1] = max;
  }

  void SplatType::compile(Cx &cx, Op &op, Env &env, Ops &out, Opts &opts) const {
    auto &p(op.pos);
    auto &args(op.args);
    for (int i(0); i < 2; i ++) { args[i].compile(p, env, opts); }

    if (Val &max(args[1]); max.type != &cx.int_type) {
      throw ESys(p, "Invalid max: ", max);
    }

    out.push_back(op);
  }
  
  void SplatType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);

    auto val(read_val(cx, pos, in));
    if (!val || val->is_eop()) { throw ESys(p, "Missing val"); }

    Val max(cx.int_type, Int(-1));
    auto v(read_val(cx, pos, in));

    if (!v->is_eop()) {
      max = *v;
      read_eop(pos, in);
    }
    
    out.emplace_back(cx, p, *this, *val, max);
  }
}
