#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/typeof.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const TypeofType Typeof("typeof");

  TypeofType::TypeofType(const string &id): OpType(id) {}

  void TypeofType::init(Cx &cx, Op &op, const Val &val) const { op.args[0] = val; }

  void TypeofType::compile(Cx &cx,
                           OpIter &in,
                           const OpIter &end,
                           Env &env,
                           Ops &out,
                           Opts &opts) const {
    auto &p(in->pos);
    in->args[0].compile(p, env, opts);
    out.push_back(*in);
  }

  bool TypeofType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    if (!op.args[0].eval(p, env, regs)) { return false; }
    auto &v(cx.peek(p));
    v.reset(cx.meta_type, dynamic_cast<Type *>(v.type));
    return true;
  }

  bool TypeofType::find_op(Op &op, function<bool (Ops &, OpIter &)> pred) const {
    return op.args[0].find_op(pred);
  }

  void TypeofType::mark_refs(Op &op) const { op.args[0].mark_refs(); }

  void TypeofType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto v(read_val(cx, pos, in));
    if (!v) { throw ESys(p, "Missing ;"); }

    if (v->is_eop()) {
      v = cx.$;
    } else {
      read_eop(pos, in);
    }

    out.emplace_back(cx, p, *this, *v);
  }
}
