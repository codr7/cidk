#ifndef CIDK_CX_HPP
#define CIDK_CX_HPP

#include <string>
#include <unordered_map>

#include "cidk/env.hpp"
#include "cidk/pool.hpp"
#include "cidk/sym.hpp"
#include "cidk/types/any.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/byte.hpp"
#include "cidk/types/fun.hpp"
#include "cidk/types/int.hpp"
#include "cidk/types/meta.hpp"
#include "cidk/var.hpp"

namespace cidk {
  struct Call;
  struct Ref;
  
  struct Cx {
    Pool<Env> env_pool;
    Pool<Sym> sym_pool;    
    Pool<Var> var_pool;

    unordered_map<string, const Sym *> syms;
    
    list<Ref *> refs;
    vector<Type *> types;
    list<Env *> envs;
    Env env;
    
    MetaType &Meta;
    AnyType &Any;
    BoolType &Bool;
    ByteType &Byte;
    FunType &Fun;
    IntType &Int;

    Stack stack;    
    Call *call;
    
    Cx();
    ~Cx();

    void init_types(const Pos &pos);

    const Sym *intern(const string &name);
    void compile(const Pos &pos, const Stack &in, Ops &out);
    void eval(const Pos &pos, const Ops &in);
    void mark_refs(const Pos &pos);
    void sweep_refs(const Pos &pos);
  };

  template <typename...Rest>
  Fun &Env::add_fun(const Pos &pos,
                    const string &id,
                    initializer_list<Arg> args,
                    initializer_list<Ret> rets,
                    Rest &&...rest) {
    Fun *f(cx.Fun.pool.get(cx, pos, id, args, rets, forward<Rest>(rest)...));
    set(pos, cx.intern(id), Val(pos, cx.Fun, f), true);
    return *f;
  }

  template <typename TypeT, typename...Rest>
  TypeT &Env::add_type(const Pos &pos,
                       const string &id,
                       Rest &&...rest) {
    TypeT *t(new TypeT(cx, pos, id, forward<Rest>(rest)...));
    
    set(pos,
        cx.intern(id),
        Val(pos,
            (id == "Meta") ? *dynamic_cast<MetaType *>(t) : cx.Meta,
            dynamic_cast<Type *>(t)), true);
    
    return *t;
  }
}

#endif
