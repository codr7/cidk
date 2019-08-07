#include "cidk/cx.hpp"
#include "cidk/types/env.hpp"
#include "cidk/val.hpp"

namespace cidk {
  EnvType::EnvType(Cx &cx,
                     const Pos &pos,
                     const Sym *id,
                     const vector<Type *> &parents):
    TValType<Env *>(cx, pos, id, parents) {}

  void EnvType::clone(const Pos &pos, Val &dst, const Val &src) const {
    dst.as_env = cx.env_pool.get(cx, *src.as_env);
  }

  void EnvType::cp(Val &dst, const Val &src) const { dst.as_env = src.as_env; }

  void EnvType::dump(const Val &val, ostream &out) const {
    out << '(';
    char sep(0);
    
    for (auto &v: val.as_env->items) {
      if (sep) { out << sep; }
      out << v.id << ':' << v;
      sep = ' ';
    }

    out << ')';
  }

  bool EnvType::eq(const Pos &pos, const Val &x, const Val &y) const {
    const auto &xl(x.as_env->items), &yl(y.as_env->items);
    if (xl.size() != yl.size()) { return false; }

    for (auto xi = xl.begin(), yi = yl.begin();
         xi != xl.end() || yi != yl.end();
         xi++, yi++) {
      if (xi == xl.end() || yi == yl.end() ||
          xi->id != yi->id ||
          !xi->eq(pos, *yi)) { return false; }
    }

    return true;
  }

  bool EnvType::is(const Val &x, const Val &y) const {
    return x.as_env == y.as_env;
  }

  void EnvType::mark_refs(const Val &val) const { val.as_env->mark(); }

  void EnvType::set(Val &dst, Env *val) const { dst.as_env = val; }

  void EnvType::sweep(const Pos &pos, Val &val) { val.as_env->sweep(cx, pos); }

  bool EnvType::get_bool(const Val &val) const { return !val.as_env->items.empty(); }

  Env &EnvType::get_env(Val &val) const { return *val.as_env; }
}
