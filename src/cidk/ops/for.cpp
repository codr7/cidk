#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/for.hpp"
#include "cidk/read.hpp"
#include "cidk/stack.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  struct ForData {
    Val src, var, body;
    bool push;

    ForData(const Val &src, const Val &var, const Val &body, bool push):
      src(src), var(var), body(body), push(push) {}
  };

  const ForType For("for");

  ForType::ForType(const string &id): OpType(id) {}
  
  void ForType::init(Cx &cx,
                     Op &op,
                     const Val &src,
                     const Val &var,
                     const Val &body) const {
    op.data = ForData(src, var, body, var.type == &cx.bool_type && var.as_bool);
  }

  void ForType::compile(Cx &cx,
                        OpIter &in,
                        const OpIter &end,
                        Env &env,
                        Stack &stack,
                        Ops &out,
                        Opts &opts) const {
    auto &p(in->pos);
    auto &d(in->as<ForData>());
    d.src.compile(cx, p, env, stack, opts);
    d.body.compile(cx, p, env, stack, opts);
    out.push_back(*in);
  }

  void ForType::eval(Cx &cx, Op &op, Env &env, Reg *regs, Stack &stack) const {
    auto &p(op.pos);
    auto &d(op.as<ForData>());
    d.src.eval(cx, p, env, regs, stack);
    Val src(pop(p, stack));
    
    for (Int i(0); i < src.as_int; i++) {
      if (d.push) { stack.emplace_back(cx.int_type, i); }
      d.body.eval(cx, p, env, regs, stack);
    }
  }

  void ForType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<ForData>());
    d.src.get_ids(out);
    d.body.get_ids(out);
  }

  void ForType::mark_refs(Op &op) const {
    auto &d(op.as<ForData>());
    d.src.mark_refs();
    d.body.mark_refs();
  }

  void ForType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);

    auto src(read_val(cx, pos, in));
    if (!src || src->is_eop()) { throw ESys(p, "Missing for source"); }

    auto var(read_val(cx, pos, in));
    if (!var || var->is_eop()) { throw ESys(p, "Missing for var"); }

    if (var->type != &cx.bool_type && var->type != &cx.sym_type) {
      throw ESys(p, "Invalid for var: ", var->type->id);
    }

    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing for body"); }

    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *src, *var, *body);
  }
}
