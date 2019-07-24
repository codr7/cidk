#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/set.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  struct SetData {
    Val key, val;
    bool is_expr;
    
    SetData(const Val &key, const Val &val, bool is_expr):
      key(key), val(val), is_expr(is_expr) {}
  };

  const SetType Set("set");

  SetType::SetType(const string &id): OpType(id) {}

  void SetType::init(Cx &cx, Op &op, const Val &key, const Val &val) const {
    op.data = SetData(key, val, val.type->isa(cx.expr_type));
  }

  void SetType::compile(Cx &cx,
                        OpIter &in,
                        const OpIter &end,
                        Env &env,
                        Stack &stack,
                        Ops &out,
                        Opts *opts) const {
    if (opts) { opts->env_extend = true; }
    in->as<SetData>().val.compile(cx, in->pos, env, stack, opts);
    out.push_back(*in);
  }

  void SetType::eval(Cx &cx, Op &op, Env &env, Stack &stack) const {
    const Pos &p(op.pos);
    const SetData &d(op.as<SetData>());

    Val v;
    d.val.clone(p, v);

    if (d.is_expr) {
      if (d.key.type == &cx.int_type) {
        stack.push_back(stack[d.key.as_int]);
      } else {
        stack.push_back(env.get(p, d.key.as_sym));
      }
    }
    
    v.eval(cx, p, env, stack);
    v = pop(p, stack);
  
    if (d.key.type == &cx.int_type) {
      stack[d.key.as_int] = v;
    } else {
      env.set(cx, p, d.key.as_sym, v);
    }
  }

  void SetType::get_ids(const Op &op, IdSet &out) const {
    const auto &d(op.as<SetData>());
    d.key.get_ids(out);
    d.val.get_ids(out);
  }

  void SetType::mark_refs(Op &op) const { op.as<SetData>().val.mark_refs(); }

  void SetType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    
    for (;;) {
      auto k(read_val(cx, pos, in));
      if (!k) { throw ESys(p, "Missing ;"); }
      if (k->is_eop()) { break; }

      if (k->type != &cx.int_type && k->type != &cx.sym_type) {
        throw ESys(p, "Invalid set id: ", k->type->id);
      }

      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing ;"); }
      out.emplace_back(cx, p, *this, *k, v->is_eop() ? cx.$ : *v);
    }
  }
}
