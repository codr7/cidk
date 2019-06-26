#include "cidk/conf.hpp"
#include "cidk/cx.hpp"
#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Cx::Cx():
    env(*this),
    Meta(env.add_type<MetaType>(Pos::MISSING, "Meta")),
    Any(env.add_type<AnyType>(Pos::MISSING, "Any")),
    Bool(env.add_type<BoolType>(Pos::MISSING, "Bool")),
    Byte(env.add_type<ByteType>(Pos::MISSING, "Byte")),
    Fun(env.add_type<FunType>(Pos::MISSING, "Fun")),
    Int(env.add_type<IntType>(Pos::MISSING, "Int")),
    call(nullptr) {
    init_types(Pos::MISSING);
  }

  Cx::~Cx() {
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

    if (ok != syms.end()) {
      return ok->second;
    }

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
    for (Env *e: envs) { e->mark_refs(pos); }
    for (Val v: stack) { v.mark_refs(pos); }
  }

  void Cx::sweep_refs(const Pos &pos) {
    for (auto i(refs.begin()); i != refs.end(); i++) {
      Ref *r(*i);
      
      if (r->ref_state != RefState::mark) {
        i++;
        r->sweep(pos);
        break;
      }
    }
  }
}
