#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/dump.hpp"
#include "cidk/types/nil.hpp"

namespace cidk::ops {
  const DumpType Dump("dump");

  DumpType::DumpType(string id): OpType(id) { }

  void DumpType::init(Op &op, const Val &val) const {
    op.data = val;
  }

  void DumpType::eval(const Op &op, Env &env) const {
    auto &cx(env.cx);
    auto &s(cx.stack);
    Pos p(op.pos);
    op.as<Val>().eval(env);
    auto v(*pop(p, s, false));

    if (v.type == &cx.nil_type) {
      if (s.empty()) { throw ERead(p, "Missing dump arg"); }
      v = *pop(p, s, false);
    }

    auto &out(cx.stderr);
    v.dump(p, out);
    out << endl;
  }

  void DumpType::read(Cx &cx, const Pos &pos, Reader &in, Ops &out) const {    
    Pos p(pos);
    int n(0);
    
    for (;; n++) {
      auto v(in.read_val());
      if (!v) { throw ERead(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      out.emplace_back(p, *this, *v);
    }

    if (!n) { out.emplace_back(p, *this, cx._); }
  }
}
