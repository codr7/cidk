#include <fstream>

#include "cidk/call.hpp"
#include "cidk/conf.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/op.hpp"
#include "cidk/ops/env.hpp"
#include "cidk/ops/stack.hpp"
#include "cidk/read.hpp"
#include "cidk/str.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/env.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/int.hpp"
#include "cidk/types/nil.hpp"
#include "cidk/types/list.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/types/sym.hpp"
#include "cidk/val.hpp"

namespace cidk {
  static void env_imp(Pos &pos,
                      const Macro &m,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) { out.emplace_back(pos, ops::Env); }

  static void stack_imp(Pos &pos,
                        const Macro &m,
                        istream &in,
                        Env &env,
                        Stack &stack,
                        Ops &out) { out.emplace_back(pos, ops::Stack); }

  static void Bool_imp(Call &call, Env &env, Stack &stack) {
    auto &cx(env.cx);
    auto &v(stack.back());
    v.reset(call.pos, cx.bool_type, v.type->Bool(call.pos, v));
  }
  
  Cx::Cx():
    debug(false),
    op_types(Op::types()),
    env(*this),
    meta_type(env.add_type<MetaType>(Pos::_, "Meta")),
    any_type(env.add_type<Type>(Pos::_, "Any")),
    num_type(env.add_type<Type>(Pos::_, "Num")),
    bool_type(env.add_type<BoolType>(Pos::_, "Bool", {&any_type})),
    env_type(env.add_type<EnvType>(Pos::_, "Env", {&any_type})),
    expr_type(env.add_type<ExprType>(Pos::_, "Expr", {&any_type})),
    fun_type(env.add_type<FunType>(Pos::_, "Fun", {&any_type})),
    int_type(env.add_type<IntType>(Pos::_, "Int", {&any_type, &num_type})),
    list_type(env.add_type<ListType>(Pos::_, "List", {&any_type})),
    macro_type(env.add_type<MacroType>(Pos::_, "Macro", {&any_type})),
    nil_type(env.add_type<NilType>(Pos::_, "Nil", {&any_type})),
    ostream_type(env.add_type<OStreamType>(Pos::_, "OStream", {&any_type})),
    pop_type(env.add_type<PopType>(Pos::_, "Pop", {&any_type})),
    sym_type(env.add_type<SymType>(Pos::_, "Sym", {&any_type})),
    eval_state(EvalState::go),
    call(nullptr),
    lt_id(intern("<")),
    _(nil_type),
    S(pop_type),
    T(Pos::_, bool_type, true),
    F(Pos::_, bool_type, false),
    eop(Pos::_, sym_type, intern(";")),
    stdin(&cin), stdout(&cout), stderr(&cerr) {
    libs::init_math(*this);
    env.add_const(Pos::_, "_", _);
    env.add_const(Pos::_, "$", S);
    env.add_const(Pos::_, "T", T);
    env.add_const(Pos::_, "F", F);
    
    env.add_macro(Pos::_, "env", env_imp);
    env.add_macro(Pos::_, "stack", stack_imp);
    
    env.add_fun(Pos::_, "Bool", {Arg("val")}, {Ret(bool_type)}, Bool_imp);
  }

  void Cx::deinit() {
    clear_refs();
    env.clear();
    sweep_refs(Pos::_);

#ifndef CIDK_USE_POOL
    for (auto &s: syms) { delete s.second; }
#endif
  }

  void Cx::clear_refs() {
    for (auto i(refs.next); i != &refs; i = i->next) { i->val().ref_mark = false; }
    env.ref_mark = true;
  }

  void Cx::eval(Ops &in, Env &env, Stack &stack) {
    ops.push_back(&in);
    
    for (Op &o: in) { 
      o.eval(env, stack); 
      if (eval_state != EvalState::go) { break; }
    }

    ops.pop_back();
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
    ReadState state;
    Stack stack;
    read_ops(p, f, state, *env_pool.get(env), stack, out);
  }
  
  void Cx::mark_refs() {
    clear_refs();

    for (Ops *os: ops) { cidk::mark_refs(*os); }
    for (Call *c(call); c; c = c->prev) { c->fun.mark(); }
    for (auto i(envs.next); i != &envs; i = i->next) { i->val().mark_items(); }
  }
  
  void Cx::sweep_refs(const Pos &pos) {
    for (auto i(refs.prev); i != &refs;) {
      Ref &r(i->val());
      i = i->prev;

      if (!r.ref_mark) {
        r.unlink();
        r.sweep(*this, pos);
      }
    }
  }
}
