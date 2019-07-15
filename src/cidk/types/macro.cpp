#include "cidk/sym.hpp"
#include "cidk/types/macro.hpp"
#include "cidk/val.hpp"

namespace cidk {
  MacroType::MacroType(Cx &cx,
                   const Pos &pos,
                   const Sym *id,
                   const vector<Type *> &parents):
    TValType<Macro *>(cx, pos, id, parents) {}

  void MacroType::dump(const Val &val, ostream &out) const {
    out << string("macro:") << val.as_macro->id;
  }

  void MacroType::dup(Val &dst, const Val &src) const {
    dst.as_macro = src.as_macro;
  }

  bool MacroType::is(const Val &x, const Val &y) const {
    return x.as_macro == y.as_macro;
  }

  void MacroType::mark_refs(const Val &val) const { val.as_macro->mark(); }

  void MacroType::set(const Pos &pos, Val &dst, Macro *val) const {
    dst.as_macro = val;
  }

  void MacroType::sweep(const Pos &pos, Val &val) { val.as_macro->sweep(cx, pos); }
}
