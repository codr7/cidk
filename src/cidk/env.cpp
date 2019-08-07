#include "cidk/const_type.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/env.hpp"
#include "cidk/ops/env.hpp"
#include "cidk/types/expr.hpp"

namespace cidk {  
  Env::Env(Cx &cx): Ref(cx) { cx.envs.push(*this); }

  Env::Env(Cx &cx, const Env &src): Ref(cx), items(src.items) {
    cx.envs.push(*this);
  }

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

  typename Env::Iter Env::find(const Sym *key) {
    for (auto i(items.begin()); i != items.end(); i++) {
      if (i->id >= key) { return i; }
    }

    return items.end();
  }

  Val &Env::get(const Pos &pos, const Sym *key) {
    auto i(find(key));
    if (i == items.end() || i->id != key) { throw ESys(pos, "Unknown id: ", key); }
    return *i;
  }

  void Env::let(Cx &cx, const Pos &pos, const Sym *key, const Val &val) {
    auto i(find(key));
    
    if (i == items.end() || i->id != key) {
      items.insert(i, val)->id = key;
    } else {
      throw ESys(pos, "Duplicate binding: ", key);
    }
  }

  void Env::mark() {
    if (!ref_mark) {
      ref_mark = true;
      mark_items();
    }
  }
  
  void Env::mark_items() {
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

  void Env::set(Cx &cx, const Pos &pos, const Sym *key, const Val &val) {
    auto i(find(key));
    
    if (i == items.end() || i->id != key) {
      throw ESys(pos, "Missing binding: ", key);
    }

    *i = val;
  }

  void Env::sweep(Cx &cx, const Pos &pos) {
    dynamic_cast<Ls<Env, CxEnvs> *>(this)->unlink();
    cx.env_pool.put(this);
  }

  Val *Env::try_get(const Sym *key) {
    auto i(find(key));
    return (i == items.end() || i->id != key) ? nullptr : &*i;
  }
}
