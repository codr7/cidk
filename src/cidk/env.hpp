#ifndef CIDK_ENV_HPP
#define CIDK_ENV_HPP

#include <list>
#include <map>
#include <vector>

#include "cidk/arg.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  using namespace std;

  struct Fun;
  struct Pos;
  struct Sym;
  struct Type;
  struct Val;
  struct Var;

  struct Env: Ref {
    using It = list<Env *>::iterator;

    It it;
    map<const Sym *, Var *> items;
    
    Env(Cx &cx);
    Env(const Env &src);
    Env(Env &&) = delete;

    Env &operator =(const Env &);
    Env &operator =(Env &&) = delete;

    bool add(const Pos &pos, const Sym *key, const Val &val, bool silent);
    
    template <typename...Rest>
    Fun &add_fun(const Pos &pos,
                 const string &id,
                 const vector<Arg> &args,
                 const vector<Ret> &rets,
                 Rest &&...rest);

    template <typename TypeT, typename...Rest>
    TypeT &add_type(const Pos &pos, const string &id, Rest &&...rest);

    template <typename...Rest>
    void add_var(const Pos &pos, const string &id, Rest &&...rest);

    void add_var(const Pos &pos, const string &id, const Val &val);

    void call(const Pos &pos, const Sym *id);
    
    void clear();
    bool get(const Pos &pos, const Sym *key, Val &out, bool silent);
    void mark(const Pos &pos);
    void mark_items(const Pos &pos);
    bool set(const Pos &pos, const Sym *key, const Val &val, bool force);
    virtual void sweep(const Pos &pos) override;
  };
}

#endif
