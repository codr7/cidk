#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/dump.hpp"
#include "cidk/read.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  const DumpType Dump("dump");

  DumpType::DumpType(string id): OpType(id) { }

  void DumpType::init(Op &op, const Val &val) const { op.data = val; }

  void DumpType::eval(const Op &op, Env &env, Stack &stack) const {
    auto &cx(env.cx);
    Pos p(op.pos);
    op.as<Val>().eval(p, env, stack);
    auto v(*pop(p, stack, false));
    auto &out(cx.stderr);
    v.dump(p, out);
    out << endl;
  }

  void DumpType::read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;; n++) {
      auto v(read_val(pos, in, env, stack));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      out.emplace_back(p, *this, *v);
    }

    if (!n) { out.emplace_back(p, *this, Val(cx.pop_type)); }
  }
}
