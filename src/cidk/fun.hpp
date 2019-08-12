#ifndef CIDK_FUN_HPP
#define CIDK_FUN_HPP

#include <functional>

#include "cidk/arg.hpp"
#include "cidk/def.hpp"
#include "cidk/env.hpp"
#include "cidk/fimp.hpp"
#include "cidk/ops.hpp"
#include "cidk/opts.hpp"

namespace cidk {
  struct Pos;
  
  struct Fun: Def {
    Env env;
    ArgList args;
    Fimp imp;
    Ops body;
    Opts body_opts;
    
    template <typename ArgsT, typename RetsT>
    Fun(Cx &cx,
        const Pos &pos,
        Env &env,
        const Sym *id,
        const ArgsT &args,
        const RetsT &rets,
        Fimp imp = nullptr);

    void call(Cx &cx, const Pos &pos, Env &env);
    void mark();
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
