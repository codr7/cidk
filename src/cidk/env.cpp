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

  bool Env::add(Cx &cx, const Pos &pos, const Sym *key, const Val &val, bool silent) {
    auto i(find(key));

    if (i != items.end() && i->first == key) {
      if (!silent) { throw ESys(pos, "Dup binding: ", key); }
      return false;
    }

    items.emplace(i, key, cx.env_item_pool.get(*this, val));
    return true;
  }

  void Env::add_const(Cx &cx, const Pos &pos, const string &id, const Val &val) {
    add_const(cx, pos, cx.intern(id), val);
  }

  void Env::add_const(Cx &cx, const Pos &pos, const Sym *id, const Val &val) {
    auto v(val);
    v.type = &v.type->const_type(pos);
    add(cx, pos, id, v, false);
  }

  void Env::add_const_expr(Cx &cx, const Pos &pos, const string &id, const Ops &ops) {
    add_const(cx, pos, id, Val(cx.expr_type, cx.expr_type.pool.get(cx, ops)));
  }

  void Env::add_var(Cx &cx, const Pos &pos, const string &id, const Val &val) {
    set(cx, Pos::_, cx.intern(id), val, false);
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

  void Env::restore(Cx &cx, Env &org) {
    for (auto i(items.begin()); i != items.end();) {
      if (i->second->env == this) {
        auto j(org.find(i->first));

        if (j == org.items.end() || j->first != i->first) {
          i->second->deref(cx);
          i = items.erase(i);
        } else if (i->second != j->second) {
          i->second->deref(cx);
          i->second = j->second;
          j->second->nrefs++;
          i++;
        } else {
          i++;
        }
      } else {
        i++;
      }
    }
  }

  void Env::set(Cx &cx, const Pos &pos, const Sym *key, const Val &val, bool force) {
    auto i(find(key));
    
    if (i == items.end() || i->first != key) {
      items.emplace(i, key, cx.env_item_pool.get(*this, val));
    } else {
      auto it(*i->second);
      
      if (it.env == this) {
        if (!force) { throw ESys(pos, "Dup binding: ", key); }
        it.val = val;
      } else {
        i->second = cx.env_item_pool.get(*this, val);
      }
    }
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

  void Env::use(Cx &cx, Env &src, const IdSet &ids) {
    for (auto i(ids.begin()); i != ids.end(); i++) {
      auto id(*i);
      auto it(src.try_get(id));

      if (it) {
        it->nrefs++;
        auto i(find(id));
        
        if (i == items.end() || i->first != id) {
          items.emplace(i, id, it);
        } else if (i->second != it) {
          i->second->deref(cx);
          i->second = it;
        }
      }
    }
  }
}
