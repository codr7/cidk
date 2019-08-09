#include "cidk/cx.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/types/reg.hpp"
#include "cidk/types/sym.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Val::Val(): type(nullptr) {}
  
  Val::Val(const Val &src): type(src.type) { src.cp(*this); }

  Val::Val(ValType &type): type(&type) {}

  Val::~Val() {}

  const Val &Val::operator =(const Val &src) {
    src.cp(*this);
    return *this;
  }

  Val &Val::clone(const Pos &pos, Val &dst) const {
    dst.id = id;
    dst.type = type;
    type->clone(pos, dst, *this);
    return dst;
  }

  Val &Val::cp(Val &dst) const {
    dst.id = id;
    dst.type = type;
    if (type) { type->cp(dst, *this); }
    return dst;
  }

  void Val::compile(Cx &cx, const Pos &pos, Env &env, Opts &opts) {
    type->compile(cx, pos, *this, env, opts);
  }

  void Val::dump(ostream &out) const { type->dump(*this, out); }
  
  bool Val::eq(const Pos &pos, const Val &y) const { return type->eq(pos, *this, y); }

  void Val::eval(Cx &cx, const Pos &pos, Env &env, Reg *regs) const {
    return type->eval(cx, pos, *this, env, regs);
  }
  
  bool Val::is(const Val &y) const {
    if (type != y.type) { return false; }
    return type->is(*this, y);
  }

  bool Val::is_eop() const {
    auto &cx(type->cx);
    return type == &cx.sym_type && as_sym == cx.eop.as_sym;
  }
  
  void Val::mark_refs() {
    type->mark();
    type->mark_refs(*this);
  }

  void Val::splat(const Pos &pos, Env &env) const {
    return type->splat(pos, *this, env);
  }

  void Val::sweep(const Pos &pos) { return type->sweep(pos, *this); }

  bool Val::get_bool() const { return type->get_bool(*this); }

  Env &Val::get_env() { return type->get_env(*this); }

  ostream &operator <<(ostream &out, const Val &v) {
    v.dump(out);
    return out;
  }
}
