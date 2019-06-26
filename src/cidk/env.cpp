#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/env.hpp"

namespace cidk {
  Env::Env(Cx &cx): cx(cx), it(cx.envs.insert(cx.envs.end(), this)) { }

  Env::~Env() {
    cx.envs.erase(it);
    for (auto &v: vars) { cx.var_pool.put(v.second); }
  }

  bool Env::add(const Pos &pos, const Sym *key, const Val &val, bool silent) {
    if (vars.emplace(key, cx.var_pool.get(pos, this, val)).second) { return true; }
    if (!silent) { throw DupVar(pos, key); }
    return false;
  }

  void Env::clear() {
    for (auto &v: vars) { cx.var_pool.put(v.second); }
    vars.clear();
  }
  
  bool Env::get(const Pos &pos, const Sym *key, Val &out, bool silent) {
    auto found(vars.find(key));

    if (found == vars.end()) {
      if (!silent) { throw UnknownId(pos, key); }
      return false;
    }

    out = found->second->val;
    return true;
  }

  void Env::mark_refs(const Pos &pos) {
    for (auto &v: vars) { v.second->val.mark_refs(pos); }
  }

  bool Env::set(const Pos &pos, const Sym *key, const Val &val, bool force) {
    auto found(vars.find(key));
    
    if (found == vars.end()) {
      if (!force) { return false; }
      vars.emplace(key, cx.var_pool.get(pos, this, val));
    } else {
      auto v(found->second);
      if (v->env == this) { v->val = val; }
      else { found->second = cx.var_pool.get(pos, this, val); }
    }

    return true;
  }
}
