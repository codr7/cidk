#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/gt.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const GtType Gt("gt");

  GtType::GtType(const string &id): BinOp(id, false) {}

  const Sym *GtType::get_fun_id(Cx &cx) const { return cx.intern(">"); }
}
