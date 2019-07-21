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
                       Stack &stack,
                       Ops &out,
                       Opts *opts) const { out.push_back(*in); }

  void OpType::eval(Op &op, Env &env, Stack &stack) const {}

  void OpType::get_ids(const Op &op, IdSet &out) const {}

  void OpType::mark_refs(Op &op) const {}

  void Op::compile(Cx &cx,
                   OpIter &in,
                   const OpIter &end,
                   Env &env,
                   Stack &stack,
                   Ops &out,
                   Opts *opts) {
    type->compile(cx, in, end, env, stack, out, opts);
  }

  void Op::eval(Env &env, Stack &stack) { type->eval(*this, env, stack); }

  void Op::get_ids(IdSet &out) const { type->get_ids(*this, out); }

  void Op::mark_refs() { type->mark_refs(*this); }
}
