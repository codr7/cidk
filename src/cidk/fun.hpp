#ifndef CIDK_FUN_HPP
#define CIDK_FUN_HPP

#include <functional>

#include "cidk/arg.hpp"
#include "cidk/def.hpp"
#include "cidk/stack.hpp"
#include "cidk/val.hpp"

namespace cidk {
  struct Call;
  struct Env;
  struct Pos;
  
  struct Fun: Def {
    using Imp = function<void (Call &, Env &, Stack &)>;

    Env &env;
    Imp imp;
    Val body;

    template <typename ArgsT, typename RetsT>
    Fun(Cx &cx,
        const Pos &pos,
        const Sym *id,
        const ArgsT &args,
        const RetsT &rets,
        Imp imp = nullptr);

    void mark(const Pos &pos);
    virtual void sweep(const Pos &pos) override;
  };
}

#endif
