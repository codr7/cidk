#include "cidk/call.hpp"
#include "cidk/const_type.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/val_type.hpp"

namespace cidk { 
  ValType::ValType(Cx &cx,
                   const Pos &pos,
                   const Sym *id,
                   const vector<Type *> &parents):
    Type(cx, pos, id, parents), is_const(false), _const_type(nullptr) {}

  ConstType &ValType::const_type(const Pos &pos) {
    if (!_const_type) { _const_type = new ConstType(cx, pos, *this); }
    return *_const_type;
  }

  void ValType::eval(const Pos &pos, const Val &val, Env &env, Reg *regs) const {
    val.clone(pos, cx.push(pos));
  }

  void ValType::splat(const Pos &pos, const Val &val, Env &env) const {
    cx.push(pos, val);
  }

  Env &ValType::get_env(Val &val) const { return val.type->env; }
}
