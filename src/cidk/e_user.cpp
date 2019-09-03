#include "cidk/cx.hpp"
#include "cidk/e_user.hpp"
#include "cidk/types/e.hpp"

namespace cidk {
  EUser::EUser(Cx &cx, const Pos &pos, const Val &val): Ref(cx), E(pos) {}

  void EUser::mark_refs() {
    if (!ref_mark) {
      ref_mark = true;
      val.mark_refs();
    }
  }

  void EUser::print(ostream &out) const {
    E::print(out);
    val.dump(out);
  }

  void EUser::sweep(Cx &cx, const Pos &pos) { cx.e_type.pool.put(this); }
}
