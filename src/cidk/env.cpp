#include "cidk/const_type.hpp"
#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/env.hpp"
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
    if (items.emplace(key, cx.var_pool.get(pos, this, val)).second) { return true; }
    if (!silent) { throw ESys(pos, "Dup binding: ", key); }
    return false;
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
  
  Val &Env::get(const Pos &pos, const Sym *key) {
    auto found(items.find(key));
    if (found == items.end()) { throw ESys(pos, "Unknown id: ", key); }
    return found->second->val;
  }

  void Env::mark() {
    if (!is_marked) {
      is_marked = true;
      mark_items();
    }
  }
  
  void Env::mark_items() {
    for (auto &v: items) { v.second->mark(); }
  }

  void Env::merge(Env &src) {
    src.items.insert(items.begin(), items.end());
    std::swap(src.items, items);
  }

  void Env::restore(const Pos &pos, const Env &org, bool sweep) {
    for (auto i(items.begin()); i != items.end();) {
      if (i->second->env == this) {
        auto j(org.items.find(i->first));
        
        if (sweep) {
          Ref &r(dynamic_cast<Ref &>(*i->second));
          r.unlink();
          r.sweep(cx, pos);
        }

        if (j == org.items.end()) {
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
    auto found(items.find(key));
    
    if (found == items.end()) {
      items.emplace(key, cx.var_pool.get(pos, this, val));
    } else {
      auto v(found->second);
      
      if (v->env == this) {
        if (!force) { return false; }
        v->val = val;
      } else {
        found->second = cx.var_pool.get(pos, this, val);
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
      Var &v(*i.second);

      if (v.env == this) {
        Ref &r(dynamic_cast<Ref &>(v));
        r.unlink();
        r.sweep(cx, pos);
      }
    }
  }

  optional<Val> Env::try_get(const Pos &pos, const Sym *key) {
    auto found(items.find(key));
    if (found == items.end()) { return {}; }
    return found->second->val;
  }

  void Env::use(const Pos &pos, Env &src, const IdSet &ids) {
    for (auto i(ids.begin()); i != ids.end(); i++) {
      auto id(*i);
      auto v(src.try_get(pos, id));
      if (v) { set(pos, id, *v, true); }
    }
  }
}
