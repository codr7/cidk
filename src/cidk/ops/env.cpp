#include "cidk/cx.hpp"
#include "cidk/ops/env.hpp"
#include "cidk/read.hpp"
#include "cidk/types/env.hpp"

namespace cidk::ops {
  const EnvType Env("env");

  EnvType::EnvType(const string &id): OpType(id) {}

  void EnvType::init(Op &op) const {}

  void EnvType::eval(const Op &op, cidk::Env &env, Stack &stack) const {
    stack.emplace_back(op.pos, env.cx.env_type, &env);
  }

  void EnvType::read(Cx &cx,
                     Pos &pos,
                     istream &in,
                     ReadState &state,
                     cidk::Env &env,
                     Stack &stack,
                     Ops &out) const {
    read_eop(pos, in, env, stack);
    out.emplace_back(pos, *this);
  }
}
