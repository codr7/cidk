#include "cidk/cx.hpp"
#include "cidk/str.hpp"
#include "cidk/const_type.hpp"
#include "cidk/val.hpp"

namespace cidk {
  ConstType::ConstType(Cx &cx, const Pos &pos, ValType &val_type):
    ValType(cx, pos, cx.intern(pos, str("Const", val_type.id)), {&cx.any_type}),
    val_type(val_type) {
    is_const = true;
    _const_type = this;
  }

  void ConstType::clone(const Pos &pos, Val &dst, const Val &src) const {
    val_type.clone(pos, dst, src);
    dst.type = &val_type;
  }

  void ConstType::dump(const Val &val, ostream &out) const {
    out << "Const@";
    val_type.dump(val, out);
  }
}
