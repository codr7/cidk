#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/set.hpp"
#include "cidk/read.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/reg.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  const SetType Set("set");

  SetType::SetType(const string &id): OpType(id) {}

  void SetType::init(Cx &cx, Op &op, const Val &key, const Val &val) const {
    op.args[0] = key;
    op.args[1] = val;
  }

  void SetType::compile(Cx &cx,
                        OpIter &in,
                        const OpIter &end,
                        Env &env,
                        Ops &out,
                        Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);
    args[1].compile(p, env, opts);
    auto &k(args[0]);
    
    if (k.type == &cx.sym_type) {      
      if (auto found(env.try_get(p, k.as_sym)); found && found->type->is_const) {
        throw ESys(p, "Const set: ", k);
      }

      if (auto found(opts.regs.find(k.as_sym)); found != opts.regs.end()) {
        k.reset(cx.reg_type, found->second);
      } else {
        throw ESys(p, "Unknown id: ", k);
      }
    } else if (k.type != &cx.int_type) {
        throw ESys(p, "Invalid key: ", k);
    }
                 
    out.push_back(*in);
  }

  void SetType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);
    auto &k(args[0]), &v(args[1]);

    if (v.type == &cx.expr_type) {
      cx.push(p, (k.type == &cx.int_type) ? cx.stack[k.as_int] : regs[k.as_reg]);
    }

    v.eval(p, env, regs);
  
    if (k.type == &cx.int_type) {
      cx.stack[k.as_int] = cx.pop(p);
    } else {
      regs[k.as_reg] = cx.pop(p);
    }
  }

  void SetType::mark_refs(Op &op) const { op.args[1].mark_refs(); }

  void SetType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto k(read_val(cx, pos, in));
    if (!k || k->is_eop()) { throw ESys(p, "Missing key"); }
    auto v(read_val(cx, pos, in));
    if (!v) { throw ESys(p, "Missing ;"); }

    if (v->is_eop()) {
      v = cx.$;
    } else {
      read_eop(pos, in);
    }
    
    out.emplace_back(cx, p, *this, *k, *v);
  }
}
