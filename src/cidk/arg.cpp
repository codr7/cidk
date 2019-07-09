#include "cidk/arg.hpp"
#include "cidk/e.hpp"
#include "cidk/cx.hpp"
#include "cidk/list.hpp"
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
    
    for (auto i(vs.begin()); i != vs.end(); i++) {
      const Sym *id(nullptr);

      if (i->type == &cx.sym_type) {
        id = i->as_sym;
      } else if (i->type != &cx.nil_type) {
        throw ESys(pos, "Invalid argument: ", i->type->id);
      }

      i++;
      if (i == vs.end()) { throw ESys(pos, "Odd number of arguments"); }
      Type *type(nullptr);

      if (i->type != &cx.nil_type) {
        i->eval(pos, env, stack);
        auto typev(pop(pos, stack));

        if (typev.type != &cx.meta_type) {
          throw ESys(pos, "Invalid argument type: ", typev.type->id);
        }

        type = typev.as_type;
      }

      items.emplace_back(id, type);
    }
  }

  Ret::Ret(Type &type): type(type) {}
}
