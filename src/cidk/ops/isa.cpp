#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/isa.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const IsaType Isa("isa");

  IsaType::IsaType(const string &id): OpType(id) {}

  void IsaType::init(Cx &cx, Op &op, const Val &parent, const Val &child) const {
    op.args[0] = parent;
    op.args[1] = child;
  }

  void IsaType::compile(Cx &cx, Op &op, Env &env, Ops &out, Opts &opts) const {
    auto &p(op.pos);
    auto &args(op.args);
    for (int i(0); i < 2; i++) { args[i].compile(p, env, opts); }
    out.push_back(op);
  }

  bool IsaType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);

    if (!args[1].eval(p, env, regs)) { return false; }
    Val &cv(cx.pop(p));
    if (cv.type != &cx.meta_type) { throw ESys(p, "Expected Meta: ", cv); }
    Type *child(cv.as_type);

    if (!args[0].eval(p, env, regs)) { return false; }
    Val &parent(cx.peek(p));
    if (parent.type != &cx.meta_type) { throw ESys(p, "Expected Meta: ", parent); }
      
    if (Type *pt(child->isa(*parent.as_type)); pt) {
      parent.reset(cx.meta_type, pt);
    } else {
      parent = cx._;
    }

    return true;
  }

  bool IsaType::find_op(Op &op, function<bool (Ops &, OpIter &)> pred) const {
    auto &args(op.args);
    
    for (int i(0); i < 2; i++) {
      if (args[i].find_op(pred)) { return true; }
    }
    
    return false;
  }

  void IsaType::mark_refs(Op &op) const {
    auto &args(op.args);
    for (int i(0); i < 2; i++) { args[i].mark_refs(); }
  }

  void IsaType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    Val parent(cx.$), child(cx.$);
    
    auto v(read_val(cx, pos, in));
    if (!v) { throw ESys(p, "Missing ;"); }

    if (!v->is_eop()) {
      parent = *v;

      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing ;"); }

      if (!v->is_eop()) {
        child = *v;
        read_eop(pos, in);
      }
    }

    out.emplace_back(cx, p, *this, parent, child);
  }
}
