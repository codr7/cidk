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
    auto &vs(in.items);
    const Sym *id(nullptr);
    
    for (auto i(vs.begin()); i != vs.end(); i++) {
      if (i->type == &cx.nil_type) {
        id = nullptr;
      } else if (i->type == &cx.sym_type) {
        id = i->as_sym;
      } else {
        throw ESys(pos, "Invalid argument: ", *i);
      }

      i++;
      if (i == vs.end()) { throw ESys(pos, "Invalid argument list"); }
      Type *type(nullptr);

      if (i->type != &cx.nil_type) {
        i->eval(pos, env, cx.regp);
        auto typev(cx.pop(pos));

        if (typev.type != &cx.meta_type) {
          throw ESys(pos, "Invalid argument type: ", typev);
        }

        type = typev.as_type;
      }

      items.emplace_back(id, type);
    }
  }

  Ret::Ret(Type &type): type(type) {}
}
