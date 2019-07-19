#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/div.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const DivType Div("div");

  DivType::DivType(const string &id): BinOp(id, true) {}

  const Sym *DivType::get_fun_id(Cx &cx) const { return cx.intern("/"); }
}
