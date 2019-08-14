#include "cidk/const_type.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/env.hpp"
#include "cidk/types/expr.hpp"

namespace cidk {
  void Env::add_const(Cx &cx, const Pos &pos, const string &id, const Val &val) {
    add_const(cx, pos, cx.intern(pos, id), val);
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
    let(cx, Pos::_, cx.intern(pos, id), val);
  }

  void Env::clear(Cx &cx) { items.clear(); }

  void Env::mark_refs() {
    for (auto &i: items) { i.mark_refs(); }
  }
}
