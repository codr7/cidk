#include "cidk/cx.hpp"
#include "cidk/str.hpp"
#include "cidk/const_type.hpp"
#include "cidk/val.hpp"

namespace cidk {
  ConstType::ConstType(Cx &cx, const Pos &pos, ValType &val_type):
    ValType(cx, pos, cx.intern(str("Const", val_type.id)), {&cx.any_type}),
    val_type(val_type) {
    is_const = true;
    _const_type = this;
  }

  void ConstType::clone(const Pos &pos, Val &dst, const Val &src) const {
    val_type.clone(pos, dst, src);
    dst.type = &val_type;
  }

  void ConstType::compile(const Pos &pos, Val &val, Env &env, Opts &opts) const {
    val_type.compile(pos, val, env, opts);
  }

  void ConstType::dump(const Val &val, ostream &out) const {
    out << "Const@";
    val_type.dump(val, out);
  }

  void ConstType::cp(Val &dst, const Val &src) const { val_type.cp(dst, src); }

  void ConstType::eval(const Pos &pos, const Val &val, Env &env, Reg *regs) const {
    val.clone(pos, cx.push(pos));
  }

  bool ConstType::is(const Val &x, const Val &y) const { return val_type.is(x, y); }

  void ConstType::mark_refs(const Val &val) const { val_type.mark_refs(val); }

  bool ConstType::get_bool(const Val &val) const { return val_type.get_bool(val); }

  Env &ConstType::get_env(Val &val) const { return val_type.get_env(val); }
}
