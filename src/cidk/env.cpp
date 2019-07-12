#include "cidk/const_type.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/env.hpp"
#include "cidk/env_item.hpp"
#include "cidk/ops/env.hpp"
#include "cidk/types/expr.hpp"

namespace cidk {  
  Env::Env(Cx &cx): Ref(cx), cx(cx) { cx.envs.push(*this); }

  Env::Env(const Env &src):
    Ref(src.cx), cx(src.cx), items(src.items) { cx.envs.push(*this); }

  Env &Env::operator =(const Env &src) {
    items = src.items;
    return *this;
  }

  bool Env::add(const Pos &pos, const Sym *key, const Val &val, bool silent) {
    auto i(find(key));

    if (i != items.end() && i->first == key) {
      if (!silent) { throw ESys(pos, "Dup binding: ", key); }
      return false;
    }

    items.emplace(i, key, cx.env_item_pool.get(pos, this, val));
    return true;
  }

  void Env::add_const(const Pos &pos, const string &id, const Val &val) {
    add_const(pos, cx.intern(id), val);
  }

  void Env::add_const(const Pos &pos, const Sym *id, const Val &val) {
    auto v(val);
    v.type = &v.type->const_type(pos);
    add(pos, id, v, false);
  }

  void Env::add_var(const Pos &pos, const string &id, const Val &val) {
    set(Pos::_, cx.intern(id), val, false);
  }

  void Env::clear() { items.clear(); }

  typename Env::Iter Env::find(const Sym *key) {
    for (auto i(items.begin()); i != items.end(); i++) {
      if (i->first >= key) { return i; }
    }

    return items.end();
  }

  Val &Env::get(const Pos &pos, const Sym *key) {
    auto i(find(key));
    if (i == items.end() || i->first != key) { throw ESys(pos, "Unknown id: ", key); }
    return i->second->val;
  }

  void Env::mark() {
    if (!is_marked) {
      is_marked = true;
      mark_items();
    }
  }
  
  void Env::mark_items() {
    for (auto &i: items) { i.second->mark(); }
  }

  void Env::merge(Env &src) {
    for (auto i(items.begin()), j(src.items.begin());
         j != src.items.end();
         i++, j++) {
      while (i->first < j->first) { i++; }

      if (i == items.end()) {
        copy(j, src.items.end(), back_inserter(items));
        break;
      }
      
      if (i->first == j->first) {
        i->second->val = j->second->val;
      } else {
        items.insert(i, *j);
        i++;
      }
    }
  }

  void Env::restore(const Pos &pos, Env &org, bool sweep) {
    for (auto i(items.begin()); i != items.end();) {
      if (i->second->env == this) {
        auto j(org.find(i->first));
        
        if (sweep) {
          Ref &r(dynamic_cast<Ref &>(*i->second));
          r.unlink();
          r.sweep(cx, pos);
        }

        if (j == org.items.end() || j->first != i->first) {
          i = items.erase(i);
        } else {
          i->second = j->second;
        }
      } else {
        i++;
      }
    }
  }

  bool Env::set(const Pos &pos, const Sym *key, const Val &val, bool force) {
    auto i(find(key));
    
    if (i == items.end() || i->first != key) {
      items.emplace(i, key, cx.env_item_pool.get(pos, this, val));
    } else {
      auto it(*i->second);
      
      if (it.env == this) {
        if (!force) { return false; }
        it.val = val;
      } else {
        i->second = cx.env_item_pool.get(pos, this, val);
      }
    }

    return true;
  }

  void Env::sweep(Cx &cx, const Pos &pos) {
    dynamic_cast<Ls<Env, CxEnvs> *>(this)->unlink();
    cx.env_pool.put(this);
  }

  void Env::sweep_items(const Pos &pos) {
    for (auto &i: items) {
      auto &it(*i.second);

      if (it.env == this) {
        Ref &r(dynamic_cast<Ref &>(it));
        r.unlink();
        r.sweep(cx, pos);
      }
    }
  }

  optional<Val> Env::try_get(const Pos &pos, const Sym *key) {
    auto i(find(key));
    if (i == items.end() || i->first != key) { return {}; }
    return i->second->val;
  }

  void Env::use(const Pos &pos, Env &src, const IdSet &ids) {
    for (auto i(ids.begin()); i != ids.end(); i++) {
      auto id(*i);
      auto v(src.try_get(pos, id));
      if (v) { set(pos, id, *v, true); }
    }
  }
}
