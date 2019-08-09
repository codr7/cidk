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
                        Ops &out,
                        Opts &opts) const {
    auto &p(in->pos);
    auto &d(in->as<SetData>());
    d.val.compile(p, env, opts);
    
    if (d.key.type == &cx.sym_type) {      
      if (auto found(opts.regs.find(d.key.as_sym)); found != opts.regs.end()) {
        d.reg = found->second;
      }
    }
                 
    out.push_back(*in);
  }

  void SetType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &d(op.as<SetData>());

    if (d.is_expr) {
      cx.push(p,
              (d.key.type == &cx.int_type)
              ? cx.stack[d.key.as_int]
              : regs[d.reg]);
    }

    d.val.eval(p, env, regs);
  
    if (d.key.type == &cx.int_type) {
      cx.stack[d.key.as_int] = cx.pop(p);
    } else {
      regs[d.reg] = cx.pop(p);
    }
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
