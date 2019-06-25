#include "cidk/conf.hpp"
#include "cidk/cx.hpp"
#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Cx::Cx():
    root_env(*this),
    call(nullptr),
    Any(*this, Pos::MISSING, "Any"),
    Bool(*this, Pos::MISSING, "Bool"),
    Byte(*this, Pos::MISSING, "Byte"),
    Fun(*this, Pos::MISSING, "Fun"),
    Int(*this, Pos::MISSING, "Int") {
    init_types();
  }

  Cx::~Cx() {
#ifndef CIDK_USE_POOL
    for (auto &s: syms) { delete s.second; }
#endif
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

  void Cx::init_types() {
    for (Type *t: types) { t->init(); }
    types.clear();
  }
}
