#ifndef CIDK_ENV_HPP
#define CIDK_ENV_HPP

#include <list>
#include <map>

#include "cidk/arg.hpp"

namespace cidk {
  using namespace std;

  struct Cx;
  struct Fun;
  struct Pos;
  struct Sym;
  struct Type;
  struct Val;
  struct Var;

  struct Env {
    using It = list<Env *>::iterator;

    Cx &cx;
    It it;
    map<const Sym *, Var *> vars;
    
    Env(Cx &cx);
    ~Env();

    bool add(const Pos &pos, const Sym *key, const Val &val, bool silent);
    
    template <typename...Rest>
    Fun &add_fun(const Pos &pos,
                 const string &id,
                 initializer_list<Arg> args,
                 initializer_list<Ret> rets,
                 Rest &&...rest);

    template <typename TypeT, typename...Rest>
    TypeT &add_type(const Pos &pos, const string &id, Rest &&...rest);

    void clear();
    bool get(const Pos &pos, const Sym *key, Val &out, bool silent);
    void mark_refs(const Pos &pos);
    bool set(const Pos &pos, const Sym *key, const Val &val, bool force);
  };
}

#endif
