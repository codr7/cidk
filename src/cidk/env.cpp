#include "cidk/const_type.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/env.hpp"
#include "cidk/types/expr.hpp"

namespace cidk {  
  Env::Env(Cx &cx) {}

  Env::Env(Cx &cx, const Env &src): items(src.items) {}

  void Env::add_const(Cx &cx, const Pos &pos, const string &id, const Val &val) {
    add_const(cx, pos, cx.intern(id), val);
  }

  void Env::add_const(Cx &cx, const Pos &pos, const Sym *id, const Val &val) {
    auto v(val);
    v.type = &v.type->const_type(pos);
    let(cx, pos, id, v);
  }

  void Env::add_const_expr(Cx &cx, const Pos &pos, const string &id, const Ops &ops) {
    add_const(cx, pos, id, Val(cx.expr_type, cx.expr_type.pool.get(cx, ops)));
  }

  void Env::add_var(Cx &cx, const Pos &pos, const string &id, const Val &val) {
    let(cx, Pos::_, cx.intern(id), val);
  }

  void Env::clear(Cx &cx) { items.clear(); }

  void Env::mark_refs() {
    for (auto &i: items) { i.mark_refs(); }
  }

  void Env::merge(Cx &cx, Env &src) {
    for (auto i(items.begin()), j(src.items.begin());
         j != src.items.end();
         i++, j++) {
      auto &jv(*j);
      while (i->id < jv.id) { i++; }

      if (i == items.end()) {
        for (; j != src.items.end(); j++) {
          auto &jv(*j);
          items.push_back(jv);
        }
        
        break;
      }

      auto &iv(*i);
      
      if (iv.id == jv.id) {
        if (!iv.is(jv)) { iv = jv; }
      } else {
        items.insert(i++, jv);
      }
    }
  }
}
