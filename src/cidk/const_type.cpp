#include "cidk/cx.hpp"
#include "cidk/str.hpp"
#include "cidk/const_type.hpp"
#include "cidk/val.hpp"

namespace cidk {
  ConstType::ConstType(Cx &cx, const Pos &pos, ValType &val_type):
    ValType(cx, pos, cx.intern(str("Const/", val_type.id)), {&cx.any_type}),
    val_type(val_type) {
    is_const = true;
    _const_type = this;
  }

  void ConstType::clone(const Pos &pos, Val &dst, const Val &src) const {
    val_type.clone(pos, dst, src);
    dst.type = &val_type;
  }

  void ConstType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << "const(";
    val.dump(pos, out);
    out << ')';
  }

  void ConstType::dup(Val &dst, const Val &src) const { val_type.dup(dst, src); }

  void ConstType::eval(const Pos &pos, const Val &val, Env &env, Stack &stack) const {
    Val v;
    clone(pos, v, val);
    stack.emplace_back(v);
  }

  bool ConstType::is(const Pos &pos, const Val &x, const Val &y) const {
    return val_type.is(pos, x, y);
  }

  void ConstType::mark_refs(const Pos &pos, const Val &val) {
    val_type.mark_refs(pos, val);
  }

  bool ConstType::Bool(const Pos &pos, const Val &val) const {
    return val_type.Bool(pos, val);
  }
}
