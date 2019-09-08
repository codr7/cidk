#include "cidk/arg.hpp"
#include "cidk/e.hpp"
#include "cidk/cx.hpp"
#include "cidk/list.hpp"
#include "cidk/types/list.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/sym.hpp"

namespace cidk {
  void ArgList::parse(Cx &cx,
                      const Pos &pos,
                      const List &in,
                      Env &env) {
    for (auto &a: in.items) {
      if (a.type != &cx.pair_type) { throw ESys(pos, "Invalid argument: ", a); }
      Pair &ap(*a.as_pair);
      Val &id(ap.first);
      
      if (id.type != &cx.nil_type && id.type != &cx.sym_type) {
        throw ESys(pos, "Invalid argument id: ", id);
      }

      ap.second.eval(pos, env, cx.regp);
      auto &t(cx.pop(pos));
      if (t.type != &cx.meta_type) { throw ESys(pos, "Expected type: ", t); }
      items.emplace_back((id.type == &cx.nil_type) ? nullptr : id.as_sym, t.as_type);
    }
  }

  Ret::Ret(Type &type): type(type) {}
}
