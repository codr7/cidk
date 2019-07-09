#include "cidk/op.hpp"

namespace cidk {
  unordered_map<string, OpType *> &Op::types() {
    static unordered_map<string, OpType *> m;
    return m;
  }

  OpType::OpType(const string &id): id(id) {
    Op::types().emplace(id, this);
  }
  
  void OpType::get_ids(const Op &op, IdSet &out) const {}

  void Op::eval(Env &env, Stack &stack) const {
    type->eval(*this, env, stack);
  }

  void Op::get_ids(IdSet &out) const { type->get_ids(*this, out); }
}
