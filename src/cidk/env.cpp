#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/env.hpp"

namespace cidk {
  Env::Env(Cx &cx): Ref(cx), it(cx.envs.insert(cx.envs.end(), this)) { }

  Env &Env::operator =(const Env &src) {
    ref_state = RefState::_;
    cx.envs.erase(it);    
    it = cx.envs.insert(cx.envs.end(), this);
    items = src.items;
    return *this;
  }

  bool Env::add(const Pos &pos, const Sym *key, const Val &val, bool silent) {
    if (items.emplace(key, cx.var_pool.get(pos, this, val)).second) { return true; }
    if (!silent) { throw EDupVar(pos, key); }
    return false;
  }

  void Env::add_var(const Pos &pos, const string &id, const Val &val) {
    set(Pos::_, cx.intern(id), val, false);
  }

  void Env::clear() {
    for (auto &v: items) { cx.var_pool.put(v.second); }
    items.clear();
  }
  
  bool Env::get(const Pos &pos, const Sym *key, Val &out, bool silent) {
    auto found(items.find(key));

    if (found == items.end()) {
      if (!silent) { throw EUnknownId(pos, key); }
      return false;
    }

    out = found->second->val;
    return true;
  }

  void Env::mark_refs(const Pos &pos) {
    for (auto &v: items) { v.second->val.mark_refs(pos); }
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
    for (auto &v: items) { cx.var_pool.put(v.second); }
    cx.env_pool.put(this);
  }
}
