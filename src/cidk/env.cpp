#include "cidk/const_type.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/env.hpp"
#include "cidk/env_item.hpp"
#include "cidk/ops/env.hpp"
#include "cidk/types/expr.hpp"

namespace cidk {  
  Env::Env(Cx &cx): Ref(cx) { cx.envs.push(*this); }

  Env::Env(Cx &cx, const Env &src): Ref(cx), items(src.items) {
    for (auto &i: items) { i.second->nrefs++; }
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

  void Env::clear(Cx &cx) {
    for (auto &i: items) { i.second->deref(cx); }
    items.clear();
  }

  typename Env::Iter Env::find(const Sym *key) {
    for (auto i(items.begin()); i != items.end(); i++) {
      if (i->first >= key) { return i; }
    }

    return items.end();
  }

  Val &Env::get(const Pos &pos, const Sym *key) { return get_item(pos, key).val; }

  EnvItem &Env::get_item(const Pos &pos, const Sym *key) {
    auto i(find(key));
    if (i == items.end() || i->first != key) { throw ESys(pos, "Unknown id: ", key); }
    return *i->second;
  }

  void Env::let(Cx &cx, const Pos &pos, const Sym *key, const Val &val) {
    auto i(find(key));
    
    if (i == items.end() || i->first != key) {
      items.emplace(i, key, cx.env_item_pool.get(*this, val));
    } else {
      auto &it(*i->second);
      if (it.env == this) { throw ESys(pos, "Duplicate binding: ", key); }
      i->second->deref(cx);
      i->second = cx.env_item_pool.get(*this, val);
    }
  }

  void Env::mark() {
    if (!ref_mark) {
      ref_mark = true;
      mark_items();
    }
  }
  
  void Env::mark_items() {
    for (auto &i: items) { i.second->val.mark_refs(); }
  }

  void Env::merge(Cx &cx, Env &src) {
    for (auto i(items.begin()), j(src.items.begin());
         j != src.items.end();
         i++, j++) {
      while (i->first < j->first) { i++; }

      if (i == items.end()) {
        for (; j != src.items.end(); j++) {
          items.push_back(*j);
          j->second->nrefs++;
        }
        
        break;
      }

      if (i->first == j->first) {
        if (i->second != j->second) {
          i->second->deref(cx);
          i->second = j->second;
          j->second->nrefs++;
        }
      } else {
        items.insert(i, *j);
        j->second->nrefs++;
        i++;
      }
    }
  }

  void Env::set(Cx &cx, const Pos &pos, const Sym *key, const Val &val) {
    auto i(find(key));
    
    if (i == items.end() || i->first != key) {
      throw ESys(pos, "Missing binding: ", key);
    }

    i->second->val = val;
  }

  void Env::sweep(Cx &cx, const Pos &pos) {
    for (auto &i: items) {
      auto &v(*i.second);
      if (v.env == this) { v.env = nullptr; }
      v.deref(cx);      
    }
    
    dynamic_cast<Ls<Env, CxEnvs> *>(this)->unlink();
    cx.env_pool.put(this);
  }

  EnvItem *Env::try_get(const Sym *key) {
    auto i(find(key));
    return (i == items.end() || i->first != key) ? nullptr : i->second;
  }
}
