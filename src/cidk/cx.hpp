#ifndef CIDK_CX_HPP
#define CIDK_CX_HPP

#include <iostream>
#include <string>
#include <unordered_map>

#include "cidk/env.hpp"
#include "cidk/op.hpp"
#include "cidk/path.hpp"
#include "cidk/pool.hpp"
#include "cidk/read.hpp"
#include "cidk/reg.hpp"
#include "cidk/sym.hpp"
#include "cidk/types/fun.hpp"
#include "cidk/types/meta.hpp"
#include "cidk/types/ostream.hpp"
#include "cidk/types/sym.hpp"
#include "cidk/val.hpp"

namespace cidk {
  namespace fs = experimental::filesystem;

  struct Call;
  struct CharType;
  struct BoolType;
  struct ExprType;
  struct IntType;
  struct ListType;
  struct NilType;
  struct PopType;
  struct Ref;
  struct RegType;
  struct StrType;
  
  enum struct EvalState { go, recall };

  using Stack = Val *;

  struct Cx {
    bool debug = false;
    
    Pool<Sym> sym_pool;    
    unordered_map<string, const Sym *> syms;
    
    Ls<Ref, CxRefs> refs;
    unordered_map<string, OpType *> op_types;
    Env env;
    
    MetaType &meta_type;
    Type &a_type;
    NilType &nil_type;    
    Type &any_type, &num_type;
    
    BoolType &bool_type;
    CharType &char_type;
    ExprType &expr_type;
    FunType &fun_type;
    IntType &int_type;
    ListType &list_type;
    OStreamType &ostream_type;
    PopType &pop_type;
    RegType &reg_type;
    StrType &str_type;
    SymType &sym_type;
    
    vector<Ops *> ops;
    EvalState eval_state;

    array<Reg, CIDK_REG_MAX> regs;
    Reg *regp;

    array<Val, CIDK_STACK_SIZE> stack;
    Val *stackp;
        
    Call *call;
    
    const Val _, $, T, F, eop;

    Path load_path;
    istream *stdin;
    ostream *stdout, *stderr;
    
    Cx();

    void deinit();
    void clear_refs();
    void compile(Ops &ops, Opts &opts, Env &env);
    void dump_stack(ostream &out) const;
    void eval(Ops &in, Env &env, const Opts &opts, Reg *regs);
    void eval(Ops &in, Env &env, Reg *regs);
    const Sym *intern(const string &name);

    void load(const Pos &pos,
              const Path &src,
              Read read,
              Env &env,
              Ops &out,
              Opts &opts);
    
    void mark_refs();
    
    Val &peek(const Pos &pos) {
      if (stackp == &stack[0]) { throw ESys(pos, "Stack is empty"); }
      return *(stackp - 1);
    }

    Val &pop(const Pos &pos) {
      if (stackp == &stack[0]) { throw ESys(pos, "Stack is empty"); }
      return *--stackp;
    }

    template <typename...Args>
    Val &push(const Pos &p, Args &&...args) {
      if (stackp == stack.end()) { throw ESys(p, "Stack overflow"); }
      auto &dst(*stackp);
      dst = Val(forward<Args>(args)...);
      stackp++;
      return dst;
    }

    void sweep_refs(const Pos &pos);

    optional<Val> try_pop() {
      if (stackp == &stack[0]) { return {}; }
      return *--stackp;
    }
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
    Fun *f(ft.pool.get(cx, pos, *this, id, args, rets, forward<Rest>(rest)...));
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
           Env &env,
           const Sym *id,
           const ArgsT &args,
           const RetsT &rets,
           Fimp imp): Def(cx, pos, id), env(cx, env), imp(imp) {
    for (auto a: args) {
      if (!a.id) { a.id = cx.intern(a.id_name); }
      this->args.items.push_back(a);
    }
  }

  inline bool Val::is_eop() const {
    auto &cx(type->cx);
    return type == &cx.sym_type && as_sym == cx.eop.as_sym;
  }
}

#endif
