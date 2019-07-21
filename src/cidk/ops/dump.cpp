#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/dump.hpp"
#include "cidk/read.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  const DumpType Dump("dump");

  DumpType::DumpType(const string &id): OpType(id) {}

  void DumpType::init(Cx &cx, Op &op, const Val &val) const { op.data = val; }

  void DumpType::compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Stack &stack,
                         Ops &out,
                         Opts *opts) const {
    in->as<Val>().compile(cx, in->pos, env, stack, opts);
    out.push_back(*in);
  }
  
  void DumpType::eval(Cx &cx, Op &op, Env &env, Stack &stack) const {
    const Pos &p(op.pos);
    op.as<Val>().eval(cx, p, env, stack);
    auto v(pop(p, stack));
    auto &out(*cx.stderr);
    v.dump(out);
    out << endl;
  }

  void DumpType::get_ids(const Op &op, IdSet &out) const {
    op.as<Val>().get_ids(out);
  }

  void DumpType::mark_refs(Op &op) const { op.as<Val>().mark_refs(); }

  void DumpType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;; n++) {
      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      out.emplace_back(cx, p, *this, *v);
    }

    if (!n) { out.emplace_back(cx, p, *this, Val(cx.pop_type)); }
  }
}
