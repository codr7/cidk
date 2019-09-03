#include "cidk/cx.hpp"
#include "cidk/types/reg.hpp"
#include "cidk/val.hpp"

namespace cidk {
  RegType::RegType(Cx &cx,
                   const Pos &pos,
                   Env &env,
                   const Sym *id,
                   const vector<Type *> &parents):
    TValType<size_t>(cx, pos, env, id, parents) {}

  void RegType::dump(const Val &val, ostream &out) const {
    out << val.as_reg << ":Reg";
  }

  bool RegType::eval(const Pos &pos, const Val &val, Env &env, Reg *regs) const {
    cx.push(pos, regs[val.as_reg]);
    return true;
  }

  bool RegType::is(const Val &x, const Val &y) const { return x.as_reg == y.as_reg; }

  void RegType::set(Val &dst, size_t val) const { dst.as_reg = val; }
}
