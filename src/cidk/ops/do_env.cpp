#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/do_env.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/env.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  struct Data {
    Val in, body;
    Data(const Val &in, const Val &body): in(in), body(body) { }
  };
  
  const DoEnvType DoEnv("do-env");

  DoEnvType::DoEnvType(string id): OpType(id) { }

  void DoEnvType::init(Op &op, const Val &in, const Val &body) const {
    op.data = Data(in, body);
  }
  
  void DoEnvType::eval(const Op &op, Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    Data d(op.as<Data>());
    Env *de(nullptr);
    d.in.eval(op.pos, env, stack);
    auto in(*pop(op.pos, stack, false));
    
    if (in.type == &cx.nil_type) {
      de = env.cx.env_pool.get(cx);
    } else if (in.type == &cx.bool_type && in.as_bool) {
      de = env.cx.env_pool.get(env);
    } else if (in.type == &cx.env_type) {
      de = in.as_env;
    } else {
      throw ESys(op.pos, "Invalid do-env input: ", in.type->id);
    }
    
    d.body.eval(op.pos, *de, stack);
  }

  void DoEnvType::read(Cx &cx, Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const {
    Pos p(pos);
    auto _in(read_val(pos, in, env, stack));
    if (!_in) { throw ESys(p, "Missing do-env input"); }

    auto body(read_val(pos, in, *env.cx.env_pool.get(env), stack));
    if (!body) { throw ESys(p, "Missing do-env body"); }
    read_eop(pos, in, env, stack);
    
    out.emplace_back(p, *this, *_in, *body);
  }
}
