#include "cidk/arg.hpp"
#include "cidk/e.hpp"
#include "cidk/cx.hpp"
#include "cidk/list.hpp"
#include "cidk/types/list.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/sym.hpp"

namespace cidk {
  Arg::Arg(const Sym *id, Type *type): id(id), type(type) {}

  Arg::Arg(string id, Type *type): id_name(id), id(nullptr), type(type) {}

  void ArgList::parse(Cx &cx,
                      const Pos &pos,
                      const List &in,
                      Env &env,
                      Stack &stack) {
    auto &vs(in.items);
    vector<const Sym *> ids;
    
    for (auto i(vs.begin()); i != vs.end(); i++) {
      if (i->type == &cx.sym_type) {
        ids.push_back(i->as_sym);
      } else if (i->type == &cx.list_type) {
        for (auto &id: i->as_list->items) {
          if (id.type != &cx.sym_type && id.type != &cx.nil_type) {
            throw ESys(pos, "Invalid argument id: ", id.type->id);
          }

          ids.push_back((id.type == &cx.sym_type) ? id.as_sym : nullptr);
        }
      } else if (i->type != &cx.nil_type) {
        throw ESys(pos, "Invalid argument: ", i->type->id);
      }

      i++;
      if (i == vs.end()) { throw ESys(pos, "Invalid argument list"); }
      Type *type(nullptr);

      if (i->type != &cx.nil_type) {
        i->eval(cx, pos, env, stack);
        auto typev(pop(pos, stack));

        if (typev.type != &cx.meta_type) {
          throw ESys(pos, "Invalid argument type: ", typev.type->id);
        }

        type = typev.as_type;
      }

      for (auto id: ids) { items.emplace_back(id, type); }
      ids.clear();
    }
  }

  Ret::Ret(Type &type): type(type) {}
}
