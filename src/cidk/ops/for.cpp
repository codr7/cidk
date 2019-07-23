#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/for.hpp"
#include "cidk/read.hpp"
#include "cidk/stack.hpp"
#include "cidk/types/nil.hpp"

namespace cidk::ops {
  struct ForData {
    Val src, var, body;
    
    ForData(const Val &src, const Val &var, const Val &body):
      src(src), var(var), body(body) {}
  };

  const ForType For("for");

  ForType::ForType(const string &id): OpType(id) {}
  
  void ForType::init(Cx &cx,
                    Op &op,
                    const Val &src,
                    const Val &var,
                    const Val &body) const {
    op.data = ForData(src, var, body);
  }

  void ForType::compile(Cx &cx,
                       OpIter &in,
                       const OpIter &end,
                       Env &env,
                       Stack &stack,
                       Ops &out,
                       Opts *opts) const {
    auto &d(in->as<ForData>());
    d.src.compile(cx, in->pos, env, stack, opts);
    d.var.compile(cx, in->pos, env, stack, opts);
    d.body.compile(cx, in->pos, env, stack, opts);
    out.push_back(*in);
  }

  void ForType::eval(Cx &cx, Op &op, Env &env, Stack &stack) const {
    auto &p(op.pos);
    auto &d(op.as<ForData>());
    d.src.eval(cx, p, env, stack);
    Val src(pop(p, stack));
    
    for (Int i(0); i < src.as_int; i++) {
      stack.emplace_back(cx.int_type, i);
      d.body.eval(cx, p, env, stack);
    }
  }

  void ForType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<ForData>());
    d.src.get_ids(out);
    d.var.get_ids(out);
    d.body.get_ids(out);
  }

  void ForType::mark_refs(Op &op) const {
    auto &d(op.as<ForData>());
    d.src.mark_refs();
    d.var.mark_refs();
    d.body.mark_refs();
  }

  void ForType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);

    auto src(read_val(cx, pos, in));
    if (!src || src->is_eop()) { throw ESys(p, "Missing for source"); }

    auto var(read_val(cx, pos, in));
    if (!var || var->is_eop()) { throw ESys(p, "Missing for var"); }

    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing for body"); }
    read_eop(pos, in);

    out.emplace_back(cx, p, *this, *src, *var, *body);
  }
}
