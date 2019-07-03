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

  void Val::call(const Pos &pos) {
    type->call(pos, *this);
  }

  Val &Val::clone(const Pos &pos, Val &dst) const {
    Cx &cx(type->cx);
    auto &s(cx.stack);
    s.emplace_back(*this);
    type->env.call(pos, cx.intern("clone"));
    dst = s.back();
    s.pop_back();
    return dst;
  }

  void Val::dump(const Pos &pos, ostream &out) const {
    Cx &cx(type->cx);
    auto &s(cx.stack);
    s.emplace_back(*this);
    auto &ot(cx.ostream_type);
    s.emplace_back(pos, ot, ot.pool.get(cx, out));
    type->env.call(pos, cx.intern("dump"));
  }
  
  Val &Val::dup(Val &dst) const {
    type->dup(dst, *this);
    dst.type = type;
    return dst;
  }

  bool Val::eq(const Pos &pos, const Val &y) const {
    Cx &cx(type->cx);
    auto &s(cx.stack);
    s.emplace_back(*this);
    s.emplace_back(y);
    type->env.call(pos, cx.intern("eq"));
    bool ok(s.back().as_bool);
    s.pop_back();
    return ok;
  }

  void Val::eval(const Pos &pos, Env &env) const {
    return type->eval(pos, *this, env);
  }

  bool Val::is(const Pos &pos, const Val &y) const { return type->is(pos, *this, y); }

  bool Val::is_eop() const {
    auto &cx(type->cx);
    return type == &cx.sym_type && as_sym == cx.eop.as_sym;
  }
  
  void Val::mark_refs(const Pos &pos) {
    type->ref_state = RefState::mark;
    type->mark_refs(pos, *this);
  }

  void Val::splat(const Pos &pos) { return type->splat(pos, *this); }

  void Val::sweep(const Pos &pos) { return type->sweep(pos, *this); }

  bool Val::Bool(const Pos &pos) const {
    Cx &cx(type->cx);
    auto &s(cx.stack);
    s.emplace_back(*this);
    type->env.call(pos, cx.intern("Bool"));
    bool ok(s.back().as_bool);
    s.pop_back();
    return ok;
  }
}
