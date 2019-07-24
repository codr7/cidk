#include "cidk/cx.hpp"
#include "cidk/types/list.hpp"
#include "cidk/val.hpp"

namespace cidk {
  ListType::ListType(Cx &cx,
                     const Pos &pos,
                     const Sym *id,
                     const vector<Type *> &parents):
    TValType<List *>(cx, pos, id, parents) {}

  void ListType::clone(const Pos &pos, Val &dst, const Val &src) const {
    auto dl(cx.list_type.pool.get(cx));

    for (const auto &sv: src.as_list->items) {
      Val dv;
      sv.clone(pos, dv);
      dl->items.push_back(dv);
    }
    
    dst.as_list = dl;
  }

  void ListType::cp(Val &dst, const Val &src) const { dst.as_list = src.as_list; }

  void ListType::compile(Cx &cx,
                         const Pos &pos,
                         Val &val,
                         Env &env,
                         Stack &stack,
                         Opts *opts) const {
    for (auto &v: val.as_list->items) { v.compile(cx, pos, env, stack, opts); }
  }
  
  void ListType::dump(const Val &val, ostream &out) const {
    out << '(';
    char sep(0);
    
    for (auto &v: val.as_list->items) {
      if (sep) { out << sep; }
      out << v;
      sep = ' ';
    }

    out << ')';
  }

  bool ListType::eq(const Pos &pos, const Val &x, const Val &y) const {
    const auto &xl(x.as_list->items), &yl(y.as_list->items);
    if (xl.size() != yl.size()) { return false; }

    for (auto xi = xl.begin(), yi = yl.begin();
         xi != xl.end() || yi != yl.end();
         xi++, yi++) {
      if (xi == xl.end() || yi == yl.end() || !xi->eq(pos, *yi)) { return false; }
    }

    return true;
  }

  void ListType::eval(Cx &cx,
                      const Pos &pos,
                      const Val &val,
                      Env &env,
                      Stack &stack) const {
    Stack out;
    for (auto &v: val.as_list->items) { v.eval(cx, pos, env, out); }
    List *l(cx.list_type.pool.get(cx, out));
    stack.emplace_back(cx.list_type, l);
  }

  void ListType::get_ids(const Val &val, IdSet &out) const {
    for (auto &v: val.as_list->items) { v.get_ids(out); }
  }

  bool ListType::is(const Val &x, const Val &y) const {
    return x.as_list == y.as_list;
  }

  void ListType::mark_refs(const Val &val) const { val.as_list->mark(); }

  void ListType::set(Val &dst, List *val) const { dst.as_list = val; }

  void ListType::splat(const Pos &pos, const Val &val, Env &env, Stack &stack) const {
    for (auto &v: val.as_list->items) { stack.push_back(v); }
  }

  void ListType::sweep(const Pos &pos, Val &val) { val.as_list->sweep(cx, pos); }

  bool ListType::get_bool(const Val &val) const {
    return !val.as_list->items.empty();
  }
}
