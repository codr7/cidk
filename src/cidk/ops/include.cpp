#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/include.hpp"
#include "cidk/read.hpp"
#include "cidk/types/str.hpp"

namespace cidk::ops {
  const IncludeType Include("include");

  IncludeType::IncludeType(const string &id): OpType(id) {}

  void IncludeType::init(Cx &cx, Op &op, string val) const { op.data = move(val); }

  void IncludeType::compile(Cx &cx,
                            Op &op,
                            Env &env,
                            Stack &stack,
                            Ops &out,
                            Opts *opts) const {
    cx.load(op.pos, op.as<string>(), env, stack, out, opts);    
    out.push_back(op);
  }

  void IncludeType::read(Cx &cx,
                         Pos &pos,
                         istream &in,
                         Env &env,
                         Stack &stack,
                         Ops &out) const {
    Pos p(pos);
    int n(0);

    for (;; n++) {
      Pos vp(pos);
      auto v(read_val(pos, in, env, stack));
      if (!v) { throw ESys(vp, "Missing ;"); }
      if (v->is_eop()) { break; }

      if (v->type != &cx.str_type) {
        throw ESys(vp, "Expected Str, was: ", v->type->id);
      } 

      out.emplace_back(cx, p, *this, v->as_str->to_utf8(cx));
    }

    if (!n) { throw ESys(p, "Missing Include path"); }
  }
}
