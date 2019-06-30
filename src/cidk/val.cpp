#include "cidk/cx.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Val::Val(): pos(Pos::_), type(nullptr) { }
  
  Val::Val(const Val &src): pos(src.pos), type(src.type) { src.dup(pos, *this); }

  Val::Val(const Pos &pos, ValType &type): pos(pos), type(&type) { }

  Val::~Val() { }

  const Val &Val::operator =(const Val &src) {
    src.dup(pos, *this);
    return *this;
  }

  void Val::call(const Pos &pos) {
    type->call(pos, *this);
  }

  void Val::clone(const Pos &pos, Val &dst) const {
    Cx &cx(type->cx);
    auto &s(cx.stack);
    s.emplace_back(*this);
    type->call_env(pos, cx.intern("clone"));
    dst = s.back();
    s.pop_back();
  }

  void Val::dump(const Pos &Pos, ostream &out) const {
    Cx &cx(type->cx);
    auto &s(cx.stack);
    s.emplace_back(*this);
    auto &ot(cx.ostream_type);
    s.emplace_back(pos, ot, ot.pool.get(cx, out));
    type->call_env(pos, cx.intern("dump"));
  }
  
  void Val::dup(const Pos &pos, Val &dst) const {
    type->dup(pos, dst, *this);
    dst.type = type;
  }

  bool Val::eq(const Pos &pos, const Val &y) const {
    Cx &cx(type->cx);
    auto &s(cx.stack);
    s.emplace_back(*this);
    s.emplace_back(y);
    type->call_env(pos, cx.intern("eq"));
    bool ok(s.back().as_bool);
    s.pop_back();
    return ok;
  }

  void Val::eval(Env &env) const { return type->eval(*this, env); }

  bool Val::is(const Pos &pos, const Val &y) const { return type->is(pos, *this, y); }

  bool Val::_bool(const Pos &pos) const {
    Cx &cx(type->cx);
    auto &s(cx.stack);
    s.emplace_back(*this);
    type->call_env(pos, cx.intern("bool"));
    bool ok(s.back().as_bool);
    s.pop_back();
    return ok;
  }

  void Val::mark_refs(const Pos &pos) {
    type->ref_state = RefState::mark;
    type->mark_refs(pos, *this);
  }

  void Val::splat(const Pos &pos) { return type->splat(pos, *this); }

  void Val::sweep(const Pos &pos) { return type->sweep(pos, *this); }
}
