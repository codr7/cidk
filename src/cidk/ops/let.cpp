#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/let.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/ops/splat.hpp"
#include "cidk/read.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  const LetType Let("let");

  LetType::LetType(const string &id): OpType(id) {}

  void LetType::init(Cx &cx, Op &op, const Val &id, const Val &val) const {
    op.args[0] = id;
    op.args[1] = val;
  }

  static void compile_id(Cx &cx,
                         const Pos &pos,
                         const Val &id,
                         Env &env,
                         Ops &out,
                         Opts &opts) {
    if (id.type == &cx.pair_type) {
      compile_back(cx, pos, env, out, opts, Splat, cx.$, Val(cx.int_type, Int(1)));
      auto &p(*id.as_pair);
      compile_id(cx, pos, p.second, env, out, opts);
      compile_id(cx, pos, p.first, env, out, opts);
    } else {
      compile_back(cx, pos, env, out, opts, Let, id, cx.$);
    }
  }
  
  void LetType::compile(Cx &cx, Op &op, Env &env, Ops &out, Opts &opts) const {
    auto &p(op.pos);
    auto &args(op.args);
    auto &id(args[0]), &val(args[1]);
    val.compile(p, env, opts);

    if (id.type == &cx.sym_type) {
      auto found(env.try_get(p, id.as_sym));
      if (found && found->type->is_const) { throw ESys(p, "Const let: ", id); }
      auto &ids(id.as_sym);
      id.reset(cx.reg_type, opts.push_reg(p, ids));
      id.id = ids;
      out.push_back(op);
    } else {
      if (val.type != &cx.pop_type) { 
        compile_back(cx, p, env, out, opts, Push, val); 
      }

      compile_id(cx, p, id, env, out, opts);
    }
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
