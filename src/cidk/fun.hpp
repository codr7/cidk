#ifndef CIDK_FUN_HPP
#define CIDK_FUN_HPP

#include <functional>

#include "cidk/arg.hpp"
#include "cidk/def.hpp"
#include "cidk/fimp.hpp"
#include "cidk/ops.hpp"
#include "cidk/opts.hpp"
#include "cidk/stack.hpp"

namespace cidk {
  struct Call;
  struct Env;
  struct Pos;
  
  struct Fun: Def {
    Env &env;
    ArgList args;
    Fimp imp;
    Ops body;
    IdSet body_ids;
    Opts body_opts;
    
    template <typename ArgsT, typename RetsT>
    Fun(Cx &cx,
        const Pos &pos,
        const Sym *id,
        const ArgsT &args,
        const RetsT &rets,
        Fimp imp = nullptr);

    void mark();
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
