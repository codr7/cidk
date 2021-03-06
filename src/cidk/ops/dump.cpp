#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/dump.hpp"
#include "cidk/read.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/pop.hpp"

namespace cidk::ops {
  const DumpType Dump("dump");

  DumpType::DumpType(const string &id): OpType(id) {}

  void DumpType::init(Cx &cx, Op &op, const Val &val) const { op.args[0] = val; }

  void DumpType::compile(Cx &cx, Op &op, Env &env, Ops &out, Opts &opts) const {
    op.args[0].compile(op.pos, env, opts);
    out.push_back(op);
  }
  
  bool DumpType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &out(*cx.stderr);
    if (!op.args[0].eval(p, env, regs)) { return false; }
    cx.pop(p).dump(out);
    out << endl;
    return true;
  }

  bool DumpType::find_op(Op &op, function<bool (Ops &, OpIter &)> pred) const {
    return op.args[0].find_op(pred);
  }

  void DumpType::mark_refs(Op &op) const { op.args[0].mark_refs(); }

  void DumpType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
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
