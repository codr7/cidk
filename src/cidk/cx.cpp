#include <fstream>

#include "cidk/conf.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/op.hpp"
#include "cidk/reader.hpp"
#include "cidk/str.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/int.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/list.hpp"
#include "cidk/types/meta.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/types/sym.hpp"
#include "cidk/val.hpp"

namespace cidk {
  Cx::Cx():
    op_types(Op::types()),
    env(*this),
    meta_type(env.add_type<MetaType>(Pos::_, "Meta")),
    any_type(env.add_type<Type>(Pos::_, "Any")),
    num_type(env.add_type<Type>(Pos::_, "Num")),
    bool_type(env.add_type<BoolType>(Pos::_, "Bool")),
    expr_type(env.add_type<ExprType>(Pos::_, "Expr")),
    fun_type(env.add_type<FunType>(Pos::_, "Fun")),
    int_type(env.add_type<IntType>(Pos::_, "Int")),
    list_type(env.add_type<ListType>(Pos::_, "List")),
    nil_type(env.add_type<NilType>(Pos::_, "Nil")),
    ostream_type(env.add_type<OStreamType>(Pos::_, "OStream")),
    pop_type(env.add_type<PopType>(Pos::_, "Pop")),
    sym_type(env.add_type<SymType>(Pos::_, "Sym")),
    eval_state(EvalState::go),
    call(nullptr),
    _(Pos::_, nil_type),
    T(Pos::_, bool_type, true),
    F(Pos::_, bool_type, false),
    eop(Pos::_, sym_type, intern(";")),
    stdin(cin), stdout(cout), stderr(cerr) {
    libs::init_math(*this);
    init_types(Pos::_);
    env.add_var(Pos::_, "_", _);
    env.add_var(Pos::_, "T", T);
    env.add_var(Pos::_, "F", F);
  }

  Cx::~Cx() {
    env.clear();
    stack.clear();

    while (!refs.empty()) {
      mark(Pos::_);
      if (!sweep(Pos::_)) { break; }
    }

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
    for (const Op &o: in) { 
      o.eval(env); 
      if (eval_state != EvalState::go) { break; }
    }
  }

  void Cx::load(const Pos &pos, const string &path, Ops &out) {
    ifstream f(path);
    if (f.fail()) { throw ESys(pos, str("File not found: ", path)); }
    Reader(*this, Pos(path), f).read_ops(*env_pool.get(env), out);
  }

  void Cx::mark(const Pos &pos) {
    for (Ref *r: refs) { r->ref_state = RefState::_; }

    env.ref_state = RefState::mark;
    for (Env *e: envs) { e->mark_items(pos); }
    for (Val &v: stack) { v.mark_refs(pos); }
  }
  
  bool Cx::sweep(const Pos &pos) {
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
