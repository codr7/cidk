#include "cidk/cx.hpp"
#include "cidk/types/fix.hpp"
#include "cidk/val.hpp"

namespace cidk {
  FixType::FixType(Cx &cx,
                   const Pos &pos,
                   Env &env,
                   const Sym *id,
                   const vector<Type *> &parents):
    TValType<Fix>(cx, pos, env, id, parents) {}

  int FixType::cmp(const Pos &pos, const Val &x, const Val &y) const {
    return fix::cmp(pos, x.as_fix, y.as_fix);
  }

  void FixType::dump(const Val &val, ostream &out) const {
    fix::dump(val.as_fix, out);
  } 

  bool FixType::is(const Val &x, const Val &y) const { return x.as_fix == y.as_fix; }

  void FixType::set(Val &dst, Fix val) const { dst.as_fix = val; }

  bool FixType::get_bool(const Val &val) const { return fix::get(val.as_fix) > 0; }
}
