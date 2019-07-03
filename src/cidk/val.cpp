#include "cidk/cx.hpp"
#include "cidk/types/sym.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Val::Val(): type(nullptr) { }
  
  Val::Val(const Val &src): type(src.type) { src.dup(*this); }

  Val::Val(ValType &type): type(&type) { }

  Val::~Val() { }

  const Val &Val::operator =(const Val &src) {
    src.dup(*this);
    return *this;
  }

  void Val::call(const Pos &pos, Env &env, Stack &stack) {
    type->call(pos, *this, env, stack);
  }

  Val &Val::clone(const Pos &pos, Val &dst) const {
    dst.type = type;
    type->clone(pos, dst, *this);
    return dst;
  }

  void Val::dump(const Pos &pos, ostream &out) const { type->dump(pos, *this, out); }
  
  Val &Val::dup(Val &dst) const {
    dst.type = type;
    type->dup(dst, *this);
    return dst;
  }

  bool Val::eq(const Pos &pos, const Val &y) const { return type->eq(pos, *this, y); }

  void Val::eval(const Pos &pos, Env &env, Stack &stack) const {
    return type->eval(pos, *this, env, stack);
  }

  bool Val::is(const Pos &pos, const Val &y) const { return type->is(pos, *this, y); }

  bool Val::is_eop() const {
    auto &cx(type->cx);
    return type == &cx.sym_type && as_sym == cx.eop.as_sym;
  }
  
  void Val::mark_refs(const Pos &pos) {
    type->is_marked = true;
    type->mark_refs(pos, *this);
  }

  void Val::splat(const Pos &pos, Env &env, Stack &stack) {
    return type->splat(pos, *this, env, stack);
  }

  void Val::sweep(const Pos &pos) { return type->sweep(pos, *this); }

  bool Val::Bool(const Pos &pos) const { return type->Bool(pos, *this); }
}
