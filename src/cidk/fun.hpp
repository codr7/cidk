#ifndef CIDK_FUN_HPP
#define CIDK_FUN_HPP

#include <functional>

#include "cidk/arg.hpp"
#include "cidk/def.hpp"
#include "cidk/env.hpp"
#include "cidk/ref.hpp"
#include "cidk/stack.hpp"

namespace cidk {
  struct Call;
  struct Pos;
  struct Val;
  
  struct Fun: Def {
    using Imp = function<void (Call &)>;

    Env env;
    Imp imp;
    
    Fun(Cx &cx,
        const Pos &pos,
        const string &id,
        initializer_list<Arg> args,
        initializer_list<Ret> rets,
        Imp imp);

    virtual void sweep(const Pos &pos) override;
  };
}

#endif
