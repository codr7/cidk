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
    Type(cx, pos, id, parents), is_const(false), _const_type(nullptr) { }

  ConstType &ValType::const_type(const Pos &pos) {
    if (!_const_type) { _const_type = new ConstType(cx, pos, *this); }
    return *_const_type;
  }

  void ValType::call(const Pos &pos, const Val &val) const {
    cx.stack.emplace_back(val);
  }

  void ValType::clone(const Pos &pos, Val &dst, const Val &src) const {
    return dup(dst, src);
  }

  bool ValType::eq(const Pos &pos, const Val &x, const Val &y) const {
    return is(pos, x, y);
  }

  void ValType::eval(const Pos &pos, const Val &val, Env &env) const {
    env.cx.stack.emplace_back(val);
  }

  void ValType::mark_refs(const Pos &pos, const Val &val) { }

  void ValType::splat(const Pos &pos, Val &val) {
    cx.stack.push_back(val);
  }

  void ValType::sweep(const Pos &pos, Val &val) { }

  bool ValType::Bool(const Pos &pos, const Val &val) const {
    return true;
  }
}
