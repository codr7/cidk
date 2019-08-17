#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/include.hpp"
#include "cidk/read.hpp"
#include "cidk/types/str.hpp"

namespace cidk::ops {
  const IncludeType Include("include");

  IncludeType::IncludeType(const string &id): OpType(id) {}

  void IncludeType::init(Cx &cx, Op &op, const Val &path) const { op.args[0] = path; }

  void IncludeType::compile(Cx &cx,
                            OpIter &in,
                            const OpIter &end,
                            Env &env,
                            Ops &out,
                            Opts &opts) const {
    auto &p(in->pos);
    auto &path(in->args[0]);
    path.compile(p, env, opts);
    if (path.type != &cx.str_type) { throw ESys(p, "Invalid path: ", path); } 
    cx.load(in->pos, path.as_str->to_utf8(cx), read_ops, env, out, opts);    
    out.push_back(*in);
  }

  void IncludeType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto v(read_val(cx, pos, in));
    if (!v || v->is_eop()) { throw ESys(p, "Missing filename"); }
    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *v);
  }
}
