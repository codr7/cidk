//#include <iostream>

#include "cidk/conf.hpp"
#include "cidk/cx.hpp"
#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Cx::Cx():
    env(*this),
    Meta(env.add_type<MetaType>(Pos::_, "Meta")),
    Any(env.add_type<AnyType>(Pos::_, "Any")),
    Bool(env.add_type<BoolType>(Pos::_, "Bool")),
    Byte(env.add_type<ByteType>(Pos::_, "Byte")),
    Fun(env.add_type<FunType>(Pos::_, "Fun")),
    Int(env.add_type<IntType>(Pos::_, "Int")),
    call(nullptr) {
    init_types(Pos::_);
  }

  Cx::~Cx() {
    env.clear();
    stack.clear();
    do { mark_refs(Pos::_); } while (sweep_refs(Pos::_));

#ifndef CIDK_USE_POOL
    for (auto &s: syms) { delete s.second; }
#endif
  }

  void Cx::init_types(const Pos &pos) {
    for (Type *t: types) { t->init(); }
    types.clear();
  }

  const Sym *Cx::intern(const string &name) {
    auto ok(syms.find(name));
    if (ok != syms.end()) { return ok->second; }
    auto s(sym_pool.get(name));
    syms.emplace(make_pair(name, s));
    return s;
  }

  void Cx::compile(const Pos &pos, const Stack &in, Ops &out) {
    for (const Val &v: in) { v.compile(pos, out); }
  }

  void Cx::eval(const Pos &pos, const Ops &in) {
    for (const Op &o: in) { o.eval(*this, pos); }
  }

  void Cx::mark_refs(const Pos &pos) {
    for (Ref *r: refs) { r->ref_state = RefState::_; }

    env.ref_state = RefState::mark;
    for (Env *e: envs) { e->mark_refs(pos); }
    for (Val &v: stack) { v.mark_refs(pos); }
  }

  bool Cx::sweep_refs(const Pos &pos) {
    bool ok(false);
    
    for (auto i(refs.begin()); i != refs.end();) {
      Ref *r(*i);

      if (r->ref_state == RefState::mark) {
        i++;
      } else {
        i = refs.erase(i);
        r->sweep(pos);
        ok = true;
      }
    }

    return ok && !refs.empty();
  }
}
