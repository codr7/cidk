#include "cidk/cx.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Val::Val(): pos(Pos::MISSING), type(nullptr) { }
  
  Val::Val(const Val &src): pos(src.pos), type(src.type) {
    src.dup(pos, *this);
  }

  Val::Val(Val &&src): pos(src.pos), type(src.type) {
    src.move(pos, *this);
  }

  Val::Val(const Pos &pos, ValType &type): pos(pos), type(&type) { }

  Val::~Val() { }

  const Val &Val::operator =(const Val &src) {
    src.dup(pos, *this);
    return *this;
  }

  const Val &Val::operator =(Val &&src) {
    src.move(pos, *this);
    return *this;
  }

  void Val::call(const Pos &pos) {
    type->call(pos, *this);
  }

  void Val::clone(const Pos &pos, Val &dst) const {
    Cx &cx(type->cx);
    auto &s(cx.stack);
    s.push_back(*this);
    type->call_env(pos, cx.intern("clone"));
    s.back().move(pos, dst);
    s.pop_back();
  }

  void Val::compile(const Pos &pos, Ops &out) const {
    type->compile(pos, *this, out);
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

  bool Val::is(const Pos &pos, const Val &y) const {
    return type->is(pos, *this, y);
  }

  void Val::move(const Pos &pos, Val &dst) {
    type->move(pos, dst, *this);
    dst.type = type;
    type = nullptr;
  }

  void Val::splat(const Pos &pos) {
    return type->splat(pos, *this);
  }

  void Val::sweep(const Pos &pos) {
    return type->sweep(pos, *this);
  }
}
