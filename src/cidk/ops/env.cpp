#include "cidk/cx.hpp"
#include "cidk/ops/env.hpp"
#include "cidk/read.hpp"
#include "cidk/types/env.hpp"

namespace cidk::ops {
  const EnvType Env("env");

  EnvType::EnvType(const string &id): OpType(id) {}

  void EnvType::init(Cx &cx, Op &op) const {}

  void EnvType::compile(Cx &cx,
                        OpIter &in,
                        const OpIter &end,
                        cidk::Env &env,
                        Stack &stack,
                        Ops &out,
                        Opts &opts) const {
    out.push_back(*in);
  }

  void EnvType::eval(Cx &cx, Op &op, cidk::Env &env, Reg *regs, Stack &stack) const {
    stack.emplace_back(cx.env_type, &env);
  }

  void EnvType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    read_eop(pos, in);
    out.emplace_back(cx, pos, *this);
  }
}
