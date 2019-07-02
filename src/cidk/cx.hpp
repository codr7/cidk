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
#include "cidk/var.hpp"

namespace cidk {
  struct Call;
  struct BoolType;
  struct ExprType;
  struct ListType;
  struct MetaType;
  struct NilType;
  struct Ref;
  struct SymType;
  
  enum struct EvalState { go, recall };
    
  struct Cx {
    Pool<Env> env_pool;
    Pool<Sym> sym_pool;    
    Pool<Var> var_pool;

    unordered_map<string, const Sym *> syms;
    
    list<Ref *> refs;
    vector<Type *> types;
    unordered_map<string, OpType *> op_types;
    list<Env *> envs;
    Env env;
    
    MetaType &meta_type;
    Type &any_type, &num_type;
    
    BoolType &bool_type;
    ByteType &byte_type;
    ExprType &expr_type;
    FunType &fun_type;
    IntType &int_type;
    ListType &list_type;
    NilType &nil_type;
    OStreamType &ostream_type;
    SymType &sym_type;

    EvalState eval_state;
    Stack stack;
    Call *call;
    
    const Val _, T, F, eop;
    
    istream &stdin;
    ostream &stdout, &stderr;
    
    Cx();
    ~Cx();

    void init_types(const Pos &pos);

    const Sym *intern(const string &name);
    void eval(const Ops &in, Env &env);
    void load(const Pos &pos, const string &path, Ops &out);
    void mark_refs(const Pos &pos);
    bool sweep_refs(const Pos &pos);
  };

  template <typename...Rest>
  Fun &Env::add_fun(const Pos &pos,
                    const string &id,
                    const vector<Arg> &args,
                    const vector<Ret> &rets,
                    Rest &&...rest) {
    auto &ft(cx.fun_type);
    Fun *f(ft.pool.get(cx, pos, cx.intern(id), args, rets, forward<Rest>(rest)...));
    set(pos, f->id, Val(pos, ft, f), false);
    return *f;
  }

  template <typename TypeT, typename...Rest>
  TypeT &Env::add_type(const Pos &pos,
                       const string &id,
                       Rest &&...rest) {
    TypeT *t(new TypeT(cx, pos, cx.intern(id), forward<Rest>(rest)...));
    
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
    Def(cx, pos, id), env(nullptr), imp(imp), body(cx._) { }
}

#endif
