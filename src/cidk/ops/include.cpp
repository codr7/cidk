#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/include.hpp"
#include "cidk/read.hpp"
#include "cidk/types/str.hpp"

namespace cidk::ops {
  const IncludeType Include("include");

  IncludeType::IncludeType(const string &id): OpType(id) {}

  void IncludeType::init(Cx &cx, Op &op, const Val &fname) const {
    op.args[0] = fname;
  }

  void IncludeType::compile(Cx &cx, Op &op, Env &env, Ops &out, Opts &opts) const {
    auto &p(op.pos);
    auto &fname(op.args[0]);
    fname.compile(p, env, opts);
    if (fname.type != &cx.str_type) { throw ESys(p, "Invalid filename: ", fname); } 
    cx.load(op.pos, fname.as_str->to_utf8(cx), read_ops, env, out, opts);    
    out.push_back(op);
  }

  void IncludeType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto v(read_val(cx, pos, in));
    if (!v || v->is_eop()) { throw ESys(p, "Missing filename"); }
    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *v);
  }
}
