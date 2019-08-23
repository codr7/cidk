#ifndef CIDK_FUN_HPP
#define CIDK_FUN_HPP

#include <functional>

#include "cidk/arg.hpp"
#include "cidk/call.hpp"
#include "cidk/def.hpp"
#include "cidk/env.hpp"
#include "cidk/fimp.hpp"
#include "cidk/int.hpp"
#include "cidk/ops.hpp"
#include "cidk/opts.hpp"

namespace cidk {
  struct Pos;
  
  struct Fun: Def {
    List &root;
    Int weight = 0;
    Env env;
    ArgList args;
    Fimp imp;
    Ops body;
    Opts body_opts;

    static List &get_root(Cx &cx, const Pos &pos, Env &env, const Sym *id);

    Fun(Cx &cx,
        const Pos &pos,
        List &root,
        Env &env,
        const Sym *id,
        const vector<Arg> &args = {},
        const vector<Ret> &rets = {},
        Fimp imp = nullptr);

    void call(Cx &cx, const Pos &pos, Env &env) {
      if (imp) {
        imp(cx, pos, *this, env);
      } else {
        Call(pos, *this).eval(cx);
      }
    }

    void init(Cx &cx, const Pos &pos, Env &env);
    void mark();
    bool match(Val *stackp, size_t stack_len) const;
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
