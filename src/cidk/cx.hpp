#ifndef CIDK_CX_HPP
#define CIDK_CX_HPP

#include <iostream>
#include <string>
#include <unordered_map>

#include "cidk/env.hpp"
#include "cidk/env_item.hpp"
#include "cidk/op.hpp"
#include "cidk/path.hpp"
#include "cidk/pool.hpp"
#include "cidk/stack.hpp"
#include "cidk/sym.hpp"
#include "cidk/types/fun.hpp"
#include "cidk/types/meta.hpp"
#include "cidk/types/ostream.hpp"

namespace cidk {
  namespace fs = experimental::filesystem;

  struct Call;
  struct BoolType;
  struct EnvType;
  struct ExprType;
  struct ListType;
  struct NilType;
  struct PopType;
  struct Ref;
  struct SexprType;
  struct StrType;
  struct SymType;
  
  enum struct EvalState { go, recall };
    
  struct Cx {
    bool debug;
    
    Pool<Env> env_pool;
    Pool<EnvItem> env_item_pool;
    Pool<Sym> sym_pool;    

    unordered_map<string, const Sym *> syms;
    
    Ls<Ref, CxRefs> refs;
    unordered_map<string, OpType *> op_types;
    Ls<Env, CxEnvs> envs;
    Env env;
    
    MetaType &meta_type;
    Type &a_type;
    NilType &nil_type;    
    Type &any_type, &num_type;
    
    BoolType &bool_type;
    CharType &char_type;
    EnvType &env_type;

    ExprType &expr_type;
    SexprType &sexpr_type;

    FunType &fun_type;
    IntType &int_type;
    ListType &list_type;
    OStreamType &ostream_type;
    PopType &pop_type;
    StrType &str_type;
    SymType &sym_type;
    
    vector<Ops *> ops;
    EvalState eval_state;
    Call *call;
    
    const Val _, $, T, F, eop;

    Path load_path;
    istream *stdin;
    ostream *stdout, *stderr;
    
    Cx();

    void deinit();
    void clear_refs();
    void compile(Ops &ops, Opts *opts, Env &env, Stack &stack);
    void eval(Ops &in, Env &env, Stack &stack);
    const Sym *intern(const string &name);

    void load(const Pos &pos,
              const Path &src,
              Env &env,
              Stack &stack,              
              Ops &out,
              Opts *opts);
    
    void mark_refs();
    void sweep_refs(const Pos &pos);
  };

  template <typename...Rest>
  Fun &Env::add_fun(Cx &cx,
                    const Pos &pos,
                    const string &id,
                    const vector<Arg> &args,
                    const vector<Ret> &rets,
                    Rest &&...rest) {
    return add_fun(cx, pos, cx.intern(id), args, rets, forward<Rest>(rest)...);
  }

  template <typename...Rest>
  Fun &Env::add_fun(Cx &cx,
                    const Pos &pos,
                    const Sym *id,
                    const vector<Arg> &args,
                    const vector<Ret> &rets,
                    Rest &&...rest) {
    auto &ft(cx.fun_type);
    Fun *f(ft.pool.get(cx, pos, id, args, rets, forward<Rest>(rest)...));
    let(cx, pos, f->id, Val(ft, f));
    return *f;
  }

  template <typename TypeT, typename...Rest>
  TypeT &Env::add_type(Cx &cx,
                       const Pos &pos,
                       const string &id,
                       const vector<Type *> parents,
                       Rest &&...rest) {
    TypeT *t(new TypeT(cx, pos, cx.intern(id), parents, forward<Rest>(rest)...));
    
    let(cx,
        pos,
        t->id,
        Val((id == "Meta") ? *dynamic_cast<MetaType *>(t) : cx.meta_type,
            dynamic_cast<Type *>(t)));
    
    return *t;
  }

  template <typename...Rest>
  void Env::add_var(Cx &cx, const Pos &pos, const string &id, Rest &&...rest) {
    add_var(cx, pos, id, Val(forward<Rest>(rest)...));
  }

  template <typename ArgsT, typename RetsT>
  Fun::Fun(Cx &cx,
           const Pos &pos,
           const Sym *id,
           const ArgsT &args,
           const RetsT &rets,
           Fimp imp):
    Def(cx, pos, id), env(*cx.env_pool.get(cx)), imp(imp) {

    for (auto a: args) {
      if (!a.id) { a.id = cx.intern(a.id_name); }
      this->args.items.push_back(a);
    }
  }
}

#endif
