#include "cidk/op.hpp"

namespace cidk {
  OpType::OpType(const string &id): id(id) { }

  void Op::eval(Cx &cx, const Pos &pos) const {
    type->eval(cx, *this);
  }
}
