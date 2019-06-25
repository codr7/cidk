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
    vector<Type *> types;

    list<Env *> envs;
    Env root_env;
    
    list<Ref *> refs;
    
    Stack stack;    
    Call *call;

    AnyType Any;
    BoolType Bool;
    ByteType Byte;
    FunType Fun;
    IntType Int;
    MetaType Meta;
    
    Cx();
    ~Cx();

    void init_types();

    const Sym *intern(const string &name);
    void compile(const Pos &pos, const Stack &in, Ops &out);
    void eval(const Pos &pos, const Ops &in);
    void mark_refs(const Pos &pos);
  };

  template <typename...Rest>
  Fun *Env::add_fun(const Pos &pos,
                    const string &id,
                    initializer_list<Arg> args,
                    initializer_list<Ret> rets,
                    Rest &&...rest) {
    Fun *f(cx.Fun.pool.get(cx, pos, id, args, rets, forward<Rest>(rest)...));
    set(pos, cx.intern(id), Val(pos, cx.Fun, f), true);
    return f;
  }
}

#endif
