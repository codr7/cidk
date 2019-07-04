#ifndef CIDK_CX_HPP
#define CIDK_CX_HPP

#include <iostream>
#include <string>
#include <unordered_map>

#include "cidk/env.hpp"
#include "cidk/op.hpp"
#include "cidk/types/ostream.hpp"
#include "cidk/pool.hpp"
#include "cidk/stack.hpp"
#include "cidk/sym.hpp"
#include "cidk/types/fun.hpp"
#include "cidk/types/macro.hpp"
#include "cidk/var.hpp"

namespace cidk {
  struct Call;
  struct BoolType;
  struct EnvType;
  struct ExprType;
  struct ListType;
  struct MetaType;
  struct NilType;
  struct PopType;
  struct Ref;
  struct SymType;
  
  enum struct EvalState { go, recall };
    
  struct Cx {
    bool debug;
    
    Pool<Env> env_pool;
    Pool<Sym> sym_pool;    
    Pool<Var> var_pool;

    unordered_map<string, const Sym *> syms;
    
    list<Ref *> refs;
    unordered_map<string, OpType *> op_types;
    list<Env *> envs;
    Env env;
    
    MetaType &meta_type;
    Type &any_type, &num_type;
    
    BoolType &bool_type;
    EnvType &env_type;
    ExprType &expr_type;
    FunType &fun_type;
    IntType &int_type;
    ListType &list_type;
    MacroType &macro_type;
    NilType &nil_type;
    OStreamType &ostream_type;
    PopType &pop_type;
    SymType &sym_type;

    EvalState eval_state;
    Call *call;
    
    const Val _, S, T, F, eop;
    
    istream *stdin;
    ostream *stdout, *stderr;
    
    Cx();

    void deinit();
    void eval(const Ops &in, Env &env, Stack &stack);
    const Sym *intern(const string &name);
    void load(const Pos &pos, const string &path, Ops &out);
    void mark(const Pos &pos);
    void sweep(const Pos &pos);
  };

  template <typename...Rest>
  Fun &Env::add_fun(const Pos &pos,
                    const string &id,
                    const vector<Arg> &args,
                    const vector<Ret> &rets,
                    Rest &&...rest) {
    auto &ft(cx.fun_type);
    
    Fun *f(ft.pool.get(cx,
                       pos,
                       cx.intern(id),
                       args, rets,
                       forward<Rest>(rest)...));
    
    set(pos, f->id, Val(pos, ft, f), false);
    return *f;
  }

  template <typename...Rest>
  Macro &Env::add_macro(const Pos &pos, const string &id, Rest &&...rest) {
    auto &t(cx.macro_type);
    
    Macro *m(t.pool.get(cx,
                        pos,
                        cx.intern(id),
                        forward<Rest>(rest)...));
    
    set(pos, m->id, Val(pos, t, m), false);
    return *m;
  }

  template <typename TypeT, typename...Rest>
  TypeT &Env::add_type(const Pos &pos,
                       const string &id,
                       const vector<Type *> parents,
                       Rest &&...rest) {
    TypeT *t(new TypeT(cx, pos, cx.intern(id), parents, forward<Rest>(rest)...));
    
    set(pos,
        t->id,
        Val(pos,
            (id == "Meta") ? *dynamic_cast<MetaType *>(t) : cx.meta_type,
            dynamic_cast<Type *>(t)),
        false);
    
    return *t;
  }

  template <typename...Rest>
  void Env::add_var(const Pos &pos, const string &id, Rest &&...rest) {
    add_var(pos, id, Val(pos, forward<Rest>(rest)...));
  }

  template <typename ArgsT, typename RetsT>
  Fun::Fun(Cx &cx,
           const Pos &pos,
           const Sym *id,
           const ArgsT &args,
           const RetsT &rets,
           Imp imp):
    Def(cx, pos, id), env(*cx.env_pool.get(cx)), imp(imp), body(cx._) { }
}

#endif
