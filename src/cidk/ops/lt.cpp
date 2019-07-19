#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/lt.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const LtType Lt("lt");

  LtType::LtType(const string &id): BinOp(id, false) {}

  const Sym *LtType::get_fun_id(Cx &cx) const { return cx.intern("<"); }
}
