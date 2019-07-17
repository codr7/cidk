#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/mul.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const MulType Mul("mul");

  MulType::MulType(const string &id): BinOp(id, true) {}

  const Sym *MulType::get_fun_id(Cx &cx) const { return cx.intern("*"); }
}
