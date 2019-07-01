#include "cidk/cx.hpp"
#include "cidk/ops/dup.hpp"

namespace cidk::ops {
  const DupType Dup("dup");

  DupType::DupType(string id): OpType(id) { }

  void DupType::init(Op &op) const { }

  void DupType::eval(const Op &op, Env &env) const {
    Stack &s(env.cx.stack);
    s.emplace_back(s.back());
  }

  void DupType::read(Cx &cx, const Pos &pos, Reader &in, Env &env, Ops &out) const {
    in.read_eop(env);
    out.emplace_back(pos, *this);
  }
}
