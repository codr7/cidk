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
    const Pos &p(op.pos);
    Data d(op.as<Data>());
    Env *de(nullptr);
    d.in.eval(p, env, stack);
    auto in(*pop(p, stack, false));
    
    if (in.type == &cx.nil_type) {
      de = env.cx.env_pool.get(cx);
    } else if (in.type == &cx.bool_type && in.as_bool) {
      de = env.cx.env_pool.get(env);
    } else if (in.type == &cx.env_type) {
      de = in.as_env;
    } else {
      throw ESys(p, "Invalid do-env input: ", in.type->id);
    }
    
    d.body.eval(p, *de, stack);
  }

  void DoEnvType::get_ids(const Op &op, IdSet &out) const {
    Data d(op.as<Data>());
    d.in.get_ids(out);
    d.body.get_ids(out);
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
