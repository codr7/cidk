#ifndef CIDK_ENV_HPP
#define CIDK_ENV_HPP

#include <map>
#include <vector>

#include "cidk/arg.hpp"
#include "cidk/ls.hpp"
#include "cidk/ref.hpp"
#include "cidk/stack.hpp"
#include "cidk/sym.hpp"

namespace cidk {
  using namespace std;

  struct EnvItem;
  struct Fun;
  struct Macro;
  struct Pos;
  struct Sym;
  struct Type;
  struct Val;

  struct CxEnvs {};
  
  struct Env: Ref, Ls<Env, CxEnvs> {
    using Items = map<const Sym *, EnvItem *>;
    
    Cx &cx;
    Items items;
    
    Env(Cx &cx);
    Env(const Env &src);
    Env(Env &&) = delete;

    Env &operator =(const Env &);
    Env &operator =(Env &&) = delete;

    bool add(const Pos &pos, const Sym *key, const Val &val, bool silent);
    void add_const(const Pos &pos, const string &id, const Val &val);
    void add_const(const Pos &pos, const Sym *id, const Val &val);

    template <typename...Rest>
    Fun &add_fun(const Pos &pos,
                 const string &id,
                 const vector<Arg> &args,
                 const vector<Ret> &rets,
                 Rest &&...rest);

    template <typename...Rest>
    Fun &add_fun(const Pos &pos,
                 const Sym *id,
                 const vector<Arg> &args,
                 const vector<Ret> &rets,
                 Rest &&...rest);

    template <typename...Rest>
    Macro &add_macro(const Pos &pos, const string &id, Rest &&...rest);

    template <typename TypeT, typename...Rest>
    TypeT &add_type(const Pos &pos,
                    const string &id,
                    const vector<Type *> parents = {},
                    Rest &&...rest);

    template <typename...Rest>
    void add_var(const Pos &pos, const string &id, Rest &&...rest);

    void add_var(const Pos &pos, const string &id, const Val &val);

    void clear();
    Val &get(const Pos &pos, const Sym *key);
    void mark();
    void mark_items();
    void merge(Env &src);
    void restore(const Pos &pos, const Env &org, bool sweep);
    bool set(const Pos &pos, const Sym *key, const Val &val, bool force);
    virtual void sweep(Cx &cx, const Pos &pos) override;
    virtual void sweep_items(const Pos &pos);
    optional<Val> try_get(const Pos &pos, const Sym *key);
    void use(const Pos &pos, Env &src, const IdSet &ids);
  };
}

#endif
