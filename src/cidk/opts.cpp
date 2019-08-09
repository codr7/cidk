#include "cidk/cidk.hpp"
#include "cidk/e.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/opts.hpp"
#include "cidk/sym.hpp"

namespace cidk {
  size_t Opts::get_reg(const Pos &pos, const Sym *id) {
    auto i(regs.find(id));
    if (i == regs.end()) { throw ESys(pos, "Unknown id: ", id); }
    return i->second;
  }

  void Opts::mark_refs() {
    for (auto &id: ext_ids) { id.mark_refs(); }
  }
  
  size_t Opts::push_ext_id(const Pos &pos, const Sym *id) {
    auto i(*try_push_reg(pos, id));
    ext_ids.emplace_back(id, i);
    return i;
  }

  size_t Opts::push_reg(const Pos &pos, const Sym *id) {
    auto i(try_push_reg(pos, id));
    if (!i) { throw ESys(pos, "Dup binding: ", id); }
    return *i;
  }

  optional<size_t> Opts::try_push_reg(const Pos &pos, const Sym *id) {
    auto i(regs.size());
    if (i == CIDK_REG_MAX) { throw ESys(pos, "Register overflow"); }
    if (regs.emplace(id, i).second) { return i; }
    return {};
  }
}

