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

  void ValType::clone(const Pos &pos, Val &dst, const Val &src) const {
    return cp(dst, src);
  }

  void ValType::compile(Cx &cx,
                        const Pos &pos,
                        Val &val,
                        Env &env,
                        Stack &stack,
                        Opts &opts) const {}

  bool ValType::eq(const Pos &pos, const Val &x, const Val &y) const {
    return is(x, y);
  }

  void ValType::eval(Cx &cx,
                     const Pos &pos,
                     const Val &val,
                     Env &env,
                     Reg *regs,
                     Stack &stack) const {
    stack.emplace_back(*val.type);
    val.clone(pos, stack.back());
  }

  void ValType::get_ids(const Val &val, IdSet &out) const {}

  void ValType::mark_refs(const Val &val) const {}

  void ValType::splat(const Pos &pos, const Val &val, Env &env, Stack &stack) const {
    stack.push_back(val);
  }

  void ValType::sweep(const Pos &pos, Val &val) {}

  bool ValType::get_bool(const Val &val) const { return true; }

  Env &ValType::get_env(Val &val) const { return val.type->env; }
}
