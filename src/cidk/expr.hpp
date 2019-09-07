#ifndef CIDK_EXPR_HPP
#define CIDK_EXPR_HPP

#include "cidk/enum.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/ops.hpp"
#include "cidk/opts.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  struct Env;
  struct Val;
  
  struct Expr: Ref {
    using Flag = Enum<Expr>;
    static Flag::Root FLAGS;
    static const Flag INLINE;
    
    Ops ops;
    Opts opts;
    Flag::Set flags;
    
    Expr(Cx &cx);
    Expr(Cx &cx, const Ops &ops);
    bool mark_refs() override;
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
