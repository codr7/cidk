#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/let.hpp"
#include "cidk/read.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  struct LetData {
    const Sym *id;
    Val val;
    Int reg;
    
    LetData(const Sym *id, const Val &val): id(id), val(val), reg(-1) {}
  };

  const LetType Let("let");

  LetType::LetType(const string &id): OpType(id) {}

  void LetType::init(Cx &cx, Op &op, const Sym *id, const Val &val) const {
    op.data = LetData(id, val);
  }

  void LetType::compile(Cx &cx,
                        OpIter &in,
                        const OpIter &end,
                        Env &env,
                        Stack &stack,
                        Ops &out,
                        Opts &opts) const {
    auto &p(in->pos);
    auto &d(in->as<LetData>());
    d.val.compile(cx, p, env, stack, opts);
    d.reg = opts.push_reg(p, d.id);
    out.push_back(*in);
  }

  void LetType::eval(Cx &cx, Op &op, Env &env, Reg *regs, Stack &stack) const {
    auto &p(op.pos);
    auto &d(op.as<LetData>());
    d.val.eval(cx, p, env, regs, stack);
    set_reg(regs, d.reg, d.id, pop(p, stack));
  }

  void LetType::get_ids(const Op &op, IdSet &out) const {
    op.as<LetData>().val.get_ids(out);
  }

  void LetType::mark_refs(Op &op) const { op.as<LetData>().val.mark_refs(); }

  void LetType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    
    for (;;) {
      auto k(read_val(cx, pos, in));
      if (!k) { throw ESys(p, "Missing ;"); }
      if (k->is_eop()) { break; }

      if (k->type != &cx.sym_type) {
        throw ESys(p, "Invalid let id: ", k->type->id);
      }

      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing ;"); }
      out.emplace_back(cx, p, *this, k->as_sym, v->is_eop() ? cx.$ : *v);
    }
  }
}
