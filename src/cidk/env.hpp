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
    map<const Sym *, Var *> bindings;
    
    Env(Cx &cx);
    ~Env();

    template <typename...Rest>
    Fun *add_fun(const Pos &pos,
                 const string &id,
                 initializer_list<Arg> args,
                 initializer_list<Ret> rets,
                 Rest &&...rest);
    
    bool get(const Pos &pos, const Sym *key, Val &out, bool silent);
    bool set(const Pos &pos, const Sym *key, const Val &val, bool force);
  };
}

#endif
