#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/dump.hpp"
#include "cidk/read.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  const DumpType Dump("dump");

  DumpType::DumpType(const string &id): OpType(id) {}

  void DumpType::init(Op &op, const Val &val) const { op.data = val; }

  void DumpType::eval(const Op &op, Env &env, Stack &stack) const {
    auto &cx(env.cx);
    const Pos &p(op.pos);
    op.as<Val>().eval(p, env, stack);
    auto v(pop(p, stack));
    auto &out(*cx.stderr);
    v.dump(out);
    out << endl;
  }

  void DumpType::get_ids(const Op &op, IdSet &out) const {
    op.as<Val>().get_ids(out);
  }

  void DumpType::mark_refs(Op &op) const { op.as<Val>().mark_refs(); }

  void DumpType::read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      ReadState &state,
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;; n++) {
      auto v(read_val(pos, in, state, env, stack));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      out.emplace_back(p, *this, *v);
    }

    if (!n) { out.emplace_back(p, *this, Val(cx.pop_type)); }
  }
}
