#include <fstream>

#include "cidk/call.hpp"
#include "cidk/conf.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/op.hpp"
#include "cidk/read.hpp"
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
  static void Bool_imp(Call &call) {
    auto &cx(call.cx);
    auto &s(cx.stack);
    auto &v(s.back());
    s.back().reset(call.pos, cx.bool_type, v.type->Bool(call.pos, v));
  }
  
  Cx::Cx():
    op_types(Op::types()),
    env(*this),
    meta_type(env.add_type<MetaType>(Pos::_, "Meta")),
    any_type(env.add_type<Type>(Pos::_, "Any")),
    num_type(env.add_type<Type>(Pos::_, "Num")),
    bool_type(env.add_type<BoolType>(Pos::_, "Bool", {&any_type})),
    expr_type(env.add_type<ExprType>(Pos::_, "Expr", {&any_type})),
    fun_type(env.add_type<FunType>(Pos::_, "Fun", {&any_type})),
    int_type(env.add_type<IntType>(Pos::_, "Int", {&any_type, &num_type})),
    list_type(env.add_type<ListType>(Pos::_, "List", {&any_type})),
    nil_type(env.add_type<NilType>(Pos::_, "Nil", {&any_type})),
    ostream_type(env.add_type<OStreamType>(Pos::_, "OStream", {&any_type})),
    pop_type(env.add_type<PopType>(Pos::_, "Pop", {&any_type})),
    sym_type(env.add_type<SymType>(Pos::_, "Sym", {&any_type})),
    eval_state(EvalState::go),
    call(nullptr),
    _(nil_type),
    S(pop_type),
    T(Pos::_, bool_type, true),
    F(Pos::_, bool_type, false),
    eop(Pos::_, sym_type, intern(";")),
    stdin(cin), stdout(cout), stderr(cerr) {
    libs::init_math(*this);
    env.add_const(Pos::_, "_", _);
    env.add_const(Pos::_, "$", S);
    env.add_const(Pos::_, "T", T);
    env.add_const(Pos::_, "F", F);
    env.add_fun(Pos::_, "Bool", {Arg("val")}, {Ret(bool_type)}, Bool_imp);
  }

  Cx::~Cx() {
    env.clear();
    stack.clear();

    while (refs.size() > 1) {
      mark(Pos::_);
      sweep(Pos::_);
    }

#ifndef CIDK_USE_POOL
    for (auto &s: syms) { delete s.second; }
#endif
  }

  void Cx::eval(const Ops &in, Env &env) {
    for (const Op &o: in) { 
      o.eval(env); 
      if (eval_state != EvalState::go) { break; }
    }
  }

  const Sym *Cx::intern(const string &name) {
    auto ok(syms.find(name));
    if (ok != syms.end()) { return ok->second; }
    auto s(sym_pool.get(name));
    syms.emplace(make_pair(name, s));
    return s;
  }

  void Cx::load(const Pos &pos, const string &path, Ops &out) {
    ifstream f(path);
    if (f.fail()) { throw ESys(pos, "File not found: ", path); }

    Pos p(path);
    read_ops(p, f, *env_pool.get(env), out);
  }

  void Cx::mark(const Pos &pos) {
    for (Ref *r: refs) { r->is_marked = false; }

    env.is_marked = true;
    for (Env *e: envs) { e->mark_items(pos); }
    for (Val &v: stack) { v.mark_refs(pos); }
  }
  
  void Cx::sweep(const Pos &pos) {
    for (auto i(refs.begin()); i != refs.end();) {
      Ref *r(*i);

      if (r->is_marked) {
        i++;
      } else {
        i = refs.erase(i);
        r->sweep(pos);
      }
    }
  }
}
