#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/let.hpp"
#include "cidk/read.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  const LetType Let("let");

  LetType::LetType(const string &id): OpType(id) {}

  void LetType::init(Cx &cx, Op &op, const Val &id, const Val &val) const {
    op.args[0] = id;
    op.args[1] = val;
  }

  static void check_id(Cx &cx, const Pos &pos, Val &id, Env &env, Opts &opts) {
    if (id.type == &cx.pair_type) {
      auto &p(*id.as_pair);
      check_id(cx, pos, p.first, env, opts);
      check_id(cx, pos, p.second, env, opts);
    } else {
      if (id.type != &cx.sym_type) { throw ESys(pos, "Invalid id: ", id); }
      auto found(env.try_get(pos, id.as_sym));
      if (found && found->type->is_const) { throw ESys(pos, "Const let: ", id); }
      
      auto &ids(id.as_sym);
      id.reset(cx.reg_type, opts.push_reg(pos, ids));
      id.id = ids;
    }
  }
  
  void LetType::compile(Cx &cx,
                        OpIter &in,
                        const OpIter &end,
                        Env &env,
                        Ops &out,
                        Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);
    check_id(cx, p, args[0], env, opts);
    args[1].compile(p, env, opts);
    out.push_back(*in);
  }

  bool LetType::find_op(Op &op, function<bool (Ops &, OpIter &)> pred) const {
    return op.args[1].find_op(pred);
  }

  void LetType::mark_refs(Op &op) const { op.args[1].mark_refs(); }

  void LetType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);    
    auto id(read_val(cx, pos, in));
    if (!id || id->is_eop()) { throw ESys(p, "Missing id"); }

    auto val(read_val(cx, pos, in));
    if (!val) { throw ESys(p, "Missing ;"); }
    
    if (val->is_eop()) {
      val = cx.$;
    } else {
      read_eop(pos, in);
    }
    
    out.emplace_back(cx, p, *this, *id, *val);
  }
}
