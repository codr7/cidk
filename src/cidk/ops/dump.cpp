#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/dump.hpp"

namespace cidk::ops {
  const DumpType Dump("dump");

  DumpType::DumpType(string id): OpType(id) { }

  void DumpType::init(Op &op, const optional<Val> &val) const {
    op.data = val;
  }

  void DumpType::eval(Cx &cx, const Op &op) const {
    auto v(op.as<optional<Val>>());

    if (!v) {
      auto &s(cx.stack);
      if (s.empty()) { ReadE(op.pos, "Missing dump arg"); }
      *v = s.back();
      s.pop_back();
    }
    
    v->dump(op.pos, cx.stderr);
    cx.stderr << endl;
  }

  void DumpType::read(Cx &cx, const Pos &pos, Reader &in, Ops &out) const {
    auto p(pos);
    out.emplace_back(p, *this, in.read_val());
  }
}
