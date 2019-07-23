#ifndef CIDK_ENV_HPP
#define CIDK_ENV_HPP

#include <vector>

#include "cidk/arg.hpp"
#include "cidk/ls.hpp"
#include "cidk/ops.hpp"
#include "cidk/ref.hpp"
#include "cidk/stack.hpp"
#include "cidk/sym.hpp"

namespace cidk {
  using namespace std;

  struct EnvItem;
  struct Fun;
  struct Pos;
  struct Sym;
  struct Type;
  struct Val;

  struct CxEnvs {};
  
  struct Env: Ref, Ls<Env, CxEnvs> {
    using Items = vector<pair<const Sym *, EnvItem *>>;
    using Iter = Items::iterator;
    
    Items items;
    
    Env(Cx &cx);
    Env(Cx &cx, const Env &src);
    Env(const Env &src) = delete;
    Env &operator =(const Env &) = delete;

    bool add(Cx &cx, const Pos &pos, const Sym *key, const Val &val, bool silent);
    void add_const(Cx &cx, const Pos &pos, const string &id, const Val &val);
    void add_const(Cx &cx, const Pos &pos, const Sym *id, const Val &val);
    void add_const_expr(Cx &cx, const Pos &pos, const string &id, const Ops &ops);
                        
    template <typename...Rest>
    Fun &add_fun(Cx &cx,
                 const Pos &pos,
                 const string &id,
                 const vector<Arg> &args,
                 const vector<Ret> &rets,
                 Rest &&...rest);

    template <typename...Rest>
    Fun &add_fun(Cx &cx,
                 const Pos &pos,
                 const Sym *id,
                 const vector<Arg> &args,
                 const vector<Ret> &rets,
                 Rest &&...rest);

    template <typename TypeT, typename...Rest>
    TypeT &add_type(Cx &cx,
                    const Pos &pos,
                    const string &id,
                    const vector<Type *> parents = {},
                    Rest &&...rest);

    template <typename...Rest>
    void add_var(Cx &cx, const Pos &pos, const string &id, Rest &&...rest);

    void add_var(Cx &cx, const Pos &pos, const string &id, const Val &val);

    void clear(Cx &cx);
    Iter find(const Sym *key);
    Val &get(const Pos &pos, const Sym *key);
    EnvItem &get_item(const Pos &pos, const Sym *key);
    void let(Cx &cx, const Pos &pos, const Sym *key, const Val &val);
    void mark();
    void mark_items();
    void merge(Cx &cx, Env &src);
    void restore(Cx &cx, Env &org);
    void set(Cx &cx, const Pos &pos, const Sym *key, const Val &val);
    virtual void sweep(Cx &cx, const Pos &pos) override;
    EnvItem *try_get(const Sym *key);
    void use(Cx &cx, Env &src, const IdSet &ids);
  };
}

#endif
