#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/env.hpp"

namespace cidk {
  Env::Env(Cx &cx): cx(cx), it(cx.envs.insert(cx.envs.end(), this)) { }

  Env::~Env() {
    cx.envs.erase(it);
    for (auto &b: bindings) { cx.var_pool.put(b.second); }
  }

  bool Env::get(const Pos &pos, const Sym *key, Val &out, bool silent) {
    auto found(bindings.find(key));

    if (found == bindings.end()) {
      if (!silent) {
        throw UnknownId(pos, key);
      }

      return false;
    }

    found->second->val.move(pos, out);
    return true;
  }
  
  bool Env::set(const Pos &pos, const Sym *key, const Val &val, bool force) {
    auto found(bindings.find(key));
    
    if (found == bindings.end()) {
      if (!force) { return false; }
      bindings.emplace(key, cx.var_pool.get(pos, this, val));
    } else {
      auto v(found->second);
      
      if (v->env == this) {
        v->val = val;
      } else {
        found->second = cx.var_pool.get(pos, this, val);
      }
    }

    return true;
  }
}
