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
    unordered_map<string, Sym *> syms;
    
    Ls<Ref> refs;
    Env env;
    
    Int type_tag = 1;
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

    array<Val, CIDK_STACK_MAX> stack;
    Val *stackp;
        
    Call *call;
    
    const Val _, $, T, F, eop;

    Path load_path;
    istream *stdin;
    ostream *stdout, *stderr;
    
    Cx();

    void deinit();
    void clear_refs();

    void compile(Ops &ops, Opts &opts, Env &env) {
      Ops tmp;
      
      for (auto i(ops.begin()); i != ops.end(); i++) {
        i->compile(*this, i, ops.end(), env, tmp, opts);
      }
      
      swap(ops, tmp);
    }

    
    void dump_stack(ostream &out) const;
    void eval(Ops &in, Env &env, Opts &opts, Reg *regs);

    void eval(Ops &in, Env &env, Reg *regs) {
      for (Op &o: in) { 
        o.eval(*this, env, regs); 
        if (eval_state != EvalState::go) { break; }
      }
    }
    
    const Sym *intern(const Pos &pos, const string &name);

    void load(const Pos &pos,
              const Path &src,
              Read read,
              Env &env,
              Ops &out,
              Opts &opts);
    
    void mark_refs();
    
    Val &peek(const Pos &pos, size_t offs = 0) {
      if (stackp - offs <= &stack[0]) { throw ESys(pos, "Stack is empty"); }
      return *(stackp - (offs + 1));
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
    return add_fun(cx, pos, cx.intern(pos, id), args, rets, forward<Rest>(rest)...);
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
    f->init(cx, pos, *this);
    let(cx, pos, f->id, Val(ft, f));
    return *f;
  }

  template <typename TypeT, typename...Rest>
  TypeT &Env::add_type(Cx &cx,
                       const Pos &pos,
                       const string &id,
                       const vector<Type *> parents,
                       Rest &&...rest) {
    TypeT *t(new TypeT(cx, pos, cx.intern(pos, id), parents, forward<Rest>(rest)...));
    
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

  inline typename Env::Iter Env::find(const Sym *id) {
    for (auto i(items.begin()); i != items.end(); i++) {
      if (i->id >= id) { return i; }
    }
    
    return items.end();
  }

  inline Val &Env::get(const Pos &pos, const Sym *id) {
    auto i(find(id));
    if (i == items.end() || i->id != id) { throw ESys(pos, "Unknown id: ", id); }
    return *i;
  }

  inline Val &Env::let(Cx &cx, const Pos &pos, const Sym *id, const Val &val) {
    auto i(find(id));

    if (i != items.end() && i->id == id) {
      throw ESys(pos, "Duplicate binding: ", id);
    }
    
    i = items.insert(i, val);
    i->id = id;
    return *i;
  }

  inline void Env::set(Cx &cx, const Pos &pos, const Sym *id, const Val &val) {
    auto i(find(id));
    
    if (i == items.end() || i->id != id) {
      throw ESys(pos, "Missing binding: ", id);
    }

    *i = val;
  }

  inline Val *Env::try_get(const Sym *id) {
    auto i(find(id));
    return (i == items.end() || i->id != id) ? nullptr : &*i;
  }

  inline bool Val::is_eop() const {
    auto &cx(type->cx);
    return type == &cx.sym_type && as_sym == cx.eop.as_sym;
  }
}

#endif
