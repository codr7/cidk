#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/sub.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const SubType Sub("sub");

  SubType::SubType(const string &id): BinOp(id, true) {}

  const Sym *SubType::get_fun_id(Cx &cx) const { return cx.intern("-"); }
}
