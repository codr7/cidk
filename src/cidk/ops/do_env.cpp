#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/do_env.hpp"
#include "cidk/read.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  const DoEnvType DoEnv("do-env");

  DoEnvType::DoEnvType(string id): OpType(id) { }

  void DoEnvType::init(Op &op, const Val &body) const { op.data = body; }
  
  void DoEnvType::eval(const Op &op, Env &env, Stack &stack) const {
    op.as<Val>().eval(op.pos, *env.cx.env_pool.get(env), stack);
  }

  void DoEnvType::read(Cx &cx, Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    Pos p(pos);
    auto body(read_val(pos, in, *env.cx.env_pool.get(env), stack));
    if (!body) { throw ESys(p, "Missing do-env body"); }
    read_eop(pos, in, env, stack);
    out.emplace_back(p, *this, *body);
  }
}
