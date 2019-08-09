#include "cidk/op.hpp"

namespace cidk {
  unordered_map<string, OpType *> &Op::types() {
    static unordered_map<string, OpType *> m;
    return m;
  }

  OpType::OpType(const string &id): id(id) {
    Op::types().emplace(id, this);
  }

  void OpType::compile(Cx &cx,
                       OpIter &in,
                       const OpIter &end,
                       Env &env,
                       Ops &out,
                       Opts &opts) const { out.push_back(*in); }

  void OpType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {}

  void OpType::mark_refs(Op &op) const {}

  void Op::compile(Cx &cx,
                   OpIter &in,
                   const OpIter &end,
                   Env &env,
                   Ops &out,
                   Opts &opts) {
    type->compile(cx, in, end, env, out, opts);
  }

  void Op::eval(Cx &cx, Env &env, Reg *regs) {
    type->eval(cx, *this, env, regs);
  }

  void Op::mark_refs() { type->mark_refs(*this); }
}
