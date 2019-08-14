#ifndef CIDK_EXPR_HPP
#define CIDK_EXPR_HPP

#include "cidk/ext_id.hpp"
#include "cidk/ops.hpp"
#include "cidk/opts.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  struct Env;
  struct Val;
  
  struct Expr: Ref {
    Ops ops;
    Opts opts;
    
    Expr(Cx &cx);
    Expr(Cx &cx, const Ops &ops);
    void mark();
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
