#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/do_env.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/env.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  struct DoEnvData {
    Val in, body;
    DoEnvData(const Val &in, const Val &body): in(in), body(body) {}
  };
  
  const DoEnvType DoEnv("do-env");

  DoEnvType::DoEnvType(const string &id): OpType(id) {}

  void DoEnvType::init(Cx &cx, Op &op, const Val &in, const Val &body) const {
    op.data = DoEnvData(in, body);
  }

  void DoEnvType::compile(Cx &cx,
                          OpIter &in,
                          const OpIter &end,
                          Env &env,
                          Stack &stack,
                          Ops &out,
                          Opts *opts) const {
    auto &d(in->as<DoEnvData>());
    d.in.compile(cx, in->pos, env, stack, opts);

    auto &ep(cx.env_pool);
    Env &de((d.in.type == &cx.nil_type) ? *ep.get(cx) : *ep.get(cx, env));

    if (de.items.empty()) { de.use(cx, env, {cx.intern("env")}); }  
    if (opts) { opts->env_escape = true; }
    d.body.compile(cx, in->pos, de, stack, opts);
    out.push_back(*in);
  }

  void DoEnvType::eval(Cx &cx, Op &op, Env &env, Stack &stack) const {
    const Pos &p(op.pos);
    const DoEnvData &d(op.as<DoEnvData>());
    Env *de(nullptr);

    if (d.in.type == &cx.nil_type) {
      de = cx.env_pool.get(cx);
    } else if (d.in.type == &cx.bool_type && d.in.as_bool) {
      de = cx.env_pool.get(cx, env);
    } else {
      d.in.eval(cx, p, env, stack);
      auto in(pop(p, stack));
      de = &in.get_env();
    }

    d.body.eval(cx, p, *de, stack);
  }

  void DoEnvType::get_ids(const Op &op, IdSet &out) const {
    DoEnvData d(op.as<DoEnvData>());
    d.in.get_ids(out);
    d.body.get_ids(out);
  }

  void DoEnvType::mark_refs(Op &op) const {
    auto &d(op.as<DoEnvData>());
    d.in.mark_refs();
    d.body.mark_refs();
  }

  void DoEnvType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto _in(read_val(cx, pos, in));
    if (!_in || _in->is_eop()) { throw ESys(p, "Missing do-env input"); }

    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing do-env body"); }

    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *_in, *body);
  }
}
