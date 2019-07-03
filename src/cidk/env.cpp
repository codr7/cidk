#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/env.hpp"

namespace cidk {
  Env::Env(Cx &cx): Ref(cx), it(cx.envs.insert(cx.envs.end(), this)) { }

  Env::Env(const Env &src):
    Ref(src.cx), it(cx.envs.insert(cx.envs.end(), this)), items(src.items) { }

  Env &Env::operator =(const Env &src) {
    items = src.items;
    return *this;
  }

  bool Env::add(const Pos &pos, const Sym *key, const Val &val, bool silent) {
    if (items.emplace(key, cx.var_pool.get(pos, this, val)).second) { return true; }
    if (!silent) { throw ESys(pos, "Dup var: ", key); }
    return false;
  }

  void Env::add_var(const Pos &pos, const string &id, const Val &val) {
    set(Pos::_, cx.intern(id), val, false);
  }

  void Env::call(const Pos &pos, const Sym *id) {
    Val target;
    get(pos, id, target, false);
    target.call(pos);
  }

  void Env::clear() { items.clear(); }
  
  bool Env::get(const Pos &pos, const Sym *key, Val &out, bool silent) {
    auto found(items.find(key));

    if (found == items.end()) {
      if (!silent) { throw ESys(pos, "Unknown id: ", key); }
      return false;
    }

    out = found->second->val;
    return true;
  }

  void Env::mark(const Pos &pos) {
    if (!is_marked) {
      is_marked = true;
      mark_items(pos);
    }
  }
  
  void Env::mark_items(const Pos &pos) {
    for (auto &v: items) { v.second->mark(pos); }
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

  void Env::sweep(const Pos &pos) {
    cx.envs.erase(it);
    cx.env_pool.put(this);
  }
}
