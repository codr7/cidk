#include <fstream>

#include "cidk/conf.hpp"
#include "cidk/cx.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/op.hpp"
#include "cidk/reader.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Cx::Cx():
    op_types(Op::types()),
    env(*this),
    meta_type(env.add_type<MetaType>(Pos::_, "Meta")),
    any_type(env.add_type<Type>(Pos::_, "Any")),
    num_type(env.add_type<Type>(Pos::_, "Num")),
    bool_type(env.add_type<BoolType>(Pos::_, "Bool")),
    byte_type(env.add_type<ByteType>(Pos::_, "Byte")),
    fun_type(env.add_type<FunType>(Pos::_, "Fun")),
    int_type(env.add_type<IntType>(Pos::_, "Int")),
    list_type(env.add_type<ListType>(Pos::_, "List")),
    ostream_type(env.add_type<OStreamType>(Pos::_, "OStream")),
    sym_type(env.add_type<SymType>(Pos::_, "Sym")),
    call(nullptr),
    stdin(cin), stdout(cout), stderr(cerr) {
    libs::init_math(*this);
    init_types(Pos::_);
    env.add_var(Pos::_, "T", bool_type, true);
    env.add_var(Pos::_, "F", bool_type, false);
  }

  Cx::~Cx() {
    env.clear();
    stack.clear();
    do { mark_refs(Pos::_); } while (sweep_refs(Pos::_) && !refs.empty());

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

  void Cx::eval(const Ops &in, Env &env) {
    for (const Op &o: in) { o.eval(env); }
  }

  void Cx::load(const Pos &pos, const string &path, Ops &out) {
    ifstream f(path);
    Reader(*this, Pos(path), f).read_ops(out);
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

    return ok;
  }
}
