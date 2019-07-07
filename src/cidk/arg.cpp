#include "cidk/arg.hpp"

namespace cidk {
  Arg::Arg(const Sym *id, Type *type): id(id), type(type) { }

  Arg::Arg(string id, Type *type): id_name(id), id(nullptr), type(type) { }

  Ret::Ret(Type &type): type(type) { }
}
