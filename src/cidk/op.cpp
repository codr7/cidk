#include "cidk/op.hpp"

namespace cidk {
  unordered_map<string, OpType *> &Op::types() {
    static unordered_map<string, OpType *> m;
    return m;
  }

  OpType::OpType(const string &id): id(id) {
    Op::types().emplace(id, this);
  }

  void Op::eval(Env &env) const {
    type->eval(*this, env);
  }
}
