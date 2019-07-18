#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/include.hpp"
#include "cidk/read.hpp"
#include "cidk/types/str.hpp"

namespace cidk::ops {
  const IncludeType Include("include");

  IncludeType::IncludeType(const string &id): OpType(id) {}

  void IncludeType::read(Cx &cx,
                         Pos &pos,
                         istream &in,
                         ReadState &state,
                         Env &env,
                         Stack &stack,
                         Ops &out) const {
    Pos p(pos);
    int n(0);

    for (;; n++) {
      Pos vp(pos);
      auto v(read_val(pos, in, state, env, stack));
      if (!v) { throw ESys(vp, "Missing ;"); }
      if (v->is_eop()) { break; }

      if (v->type != &cx.str_type) {
        throw ESys(vp, "Expected Str, was: ", v->type->id);
      } 

      cx.load(vp, v->as_str->to_utf8(cx), out);    
    }

    if (!n) { throw ESys(p, "Missing Include path"); }
  }
}
