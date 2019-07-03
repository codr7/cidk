#include "cidk/cx.hpp"
#include "cidk/types/list.hpp"
#include "cidk/val.hpp"

namespace cidk {
  ListType::ListType(Cx &cx,
                     const Pos &pos,
                     const Sym *id,
                     const vector<Type *> &parents):
    TValType<List *>(cx, pos, id, parents) { }

  void ListType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << '(';
    char sep(0);
    
    for (auto &v: val.as_list->items) {
      if (sep) { out << sep; }
      v.dump(pos, out);
      sep = ' ';
    }

    out << ')';
  }

  void ListType::dup(Val &dst, const Val &src) const { dst.as_list = src.as_list; }

  bool ListType::is(const Pos &pos, const Val &x, const Val &y) const {
    return x.as_list == y.as_list;
  }

  void ListType::mark_refs(const Pos &pos, const Val &val) { val.as_list->mark(pos); }

  void ListType::set(const Pos &pos, Val &dst, List *val) const { dst.as_list = val; }

  void ListType::splat(const Pos &pos, Val &val, Env &env, Stack &stack) {
    for (auto &v: val.as_list->items) { stack.emplace_back(v); }
  }

  void ListType::sweep(const Pos &pos, Val &val) { val.as_list->sweep(pos); }

  bool ListType::Bool(const Pos &pos, const Val &val) const {
    return !val.as_list->items.empty();
  }
}
