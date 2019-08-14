#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/let.hpp"
#include "cidk/read.hpp"
#include "cidk/types/reg.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  const LetType Let("let");

  LetType::LetType(const string &id): OpType(id) {}

  void LetType::init(Cx &cx, Op &op, const Val &id, const Val &val) const {
    op.args[0] = id;
    op.args[1] = val;
  }

  void LetType::compile(Cx &cx,
                        OpIter &in,
                        const OpIter &end,
                        Env &env,
                        Ops &out,
                        Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);
    auto &id(args[0]);
    if (id.type != &cx.sym_type) { throw ESys(p, "Invalid id: ", id); }
    args[1].compile(p, env, opts);
    args[2].reset(cx.reg_type, opts.push_reg(p, id.as_sym));
    out.push_back(*in);
  }

  void LetType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);
    args[1].eval(p, env, regs);
    regs[args[2].as_reg] = cx.pop(p);
  }

  void LetType::mark_refs(Op &op) const { op.args[1].mark_refs(); }

  void LetType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    
    for (;;) {
      auto id(read_val(cx, pos, in));
      if (!id) { throw ESys(p, "Missing ;"); }
      if (id->is_eop()) { break; }

      auto val(read_val(cx, pos, in));
      if (!val) { throw ESys(p, "Missing ;"); }
      out.emplace_back(cx, p, *this, *id, val->is_eop() ? cx.$ : *val);
    }
  }
}
