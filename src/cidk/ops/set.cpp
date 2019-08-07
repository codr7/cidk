#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/set.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  struct SetData {
    Val key, val;
    Int reg;
    bool is_expr;
    
    SetData(const Val &key, const Val &val, bool is_expr):
      key(key), val(val), reg(-1), is_expr(is_expr) {}
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
                        Opts &opts) const {
    auto &p(in->pos);
    auto &d(in->as<SetData>());
    d.val.compile(cx, p, env, stack, opts);
    
    if (d.key.type == &cx.sym_type) {      
      if (auto found(opts.regs.find(d.key.as_sym)); found != opts.regs.end()) {
        d.reg = found->second;
      }
    }
                 
    out.push_back(*in);
  }

  void SetType::eval(Cx &cx, Op &op, Env &env, Regs &regs, Stack &stack) const {
    auto &p(op.pos);
    auto &d(op.as<SetData>());

    if (d.is_expr) {
      if (d.key.type == &cx.int_type) {
        stack.push_back(stack[d.key.as_int]);
      } else {
        stack.push_back(regs[d.reg].second);
      }
    }

    d.val.push(cx, p, env, regs, stack);
  
    if (d.key.type == &cx.int_type) {
      stack[d.key.as_int] = pop(p, stack);
    } else {
      regs[d.reg].second = pop(p, stack);
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
