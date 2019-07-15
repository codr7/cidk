#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/add.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const AddType Add("add");

  AddType::AddType(const string &id): BinOp(id, true) {}

  const Sym *AddType::get_fun_id(Cx &cx) const { return cx.intern("+"); }
}
