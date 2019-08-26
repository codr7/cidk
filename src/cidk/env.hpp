#ifndef CIDK_ENV_HPP
#define CIDK_ENV_HPP

#include <vector>

#include "cidk/arg.hpp"
#include "cidk/ops.hpp"

namespace cidk {
  using namespace std;

  struct Fun;
  struct Lib;
  struct Pos;
  struct Sym;
  struct Type;
  struct Val;

  struct Env: Ls<Env> {
    using Items = vector<Val>;
    using Iter = Items::iterator;
    
    Items items;
    
    Env(Cx &cx) {}
    Env(Cx &cx, const Env &src);

    Env(const Env &src) = delete;
    Env &operator =(const Env &) = delete;

    bool add(Cx &cx, const Pos &pos, const Sym *id, const Val &val, bool silent);
    void add_const(Cx &cx, const Pos &pos, const string &id, const Val &val);
    void add_const(Cx &cx, const Pos &pos, const Sym *id, const Val &val);
    void add_const_expr(Cx &cx, const Pos &pos, const string &id, const Ops &ops);

    template <typename LibT = Lib, typename...Args>
    LibT &add_lib(Cx &cx, const Pos &pos, const string &id, Args...args);
    
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
    Iter find(const Pos &pos, const Sym *id);

    Val &get(const Pos &pos, const Sym *id);
    Iter insert(Iter i, const Sym *id, const Val &val);
    Val &let(const Pos &pos, const Sym *id, const Val &val);
    void mark_refs();

    void set(Cx &cx, const Pos &pos,
             const Sym *id,
             const Val &val,
             bool force = false);
    
    Val *try_get(const Pos &pos, const Sym *id);
    void use(Cx &cx, const Pos &pos, Env &src, const vector<const Sym *> &ids = {});
  };
}

#endif
