#include "cidk/e.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/opts.hpp"
#include "cidk/sym.hpp"

namespace cidk {
  size_t Opts::push_ext_id(const Sym *id) {
    auto i(*push_reg(id));
    ext_ids.emplace_back(id, i);
    return i;
  }

  size_t Opts::push_reg(const Pos &pos, const Sym *id) {
    auto i(push_reg(id));
    if (!i) { throw ESys(pos, "Dup binding: ", id); }
    return *i;
  }

  optional<size_t> Opts::push_reg(const Sym *id) {
    auto i(regs.size());
    if (regs.emplace(id, i).second) { return i; }
    return {};
  }
}

