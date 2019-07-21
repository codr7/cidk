#ifndef CIDK_EXPR_HPP
#define CIDK_EXPR_HPP

#include "cidk/ops.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  struct Val;
  
  struct Expr: Ref {
    Ops body;
    
    Expr(Cx &cx);
    Expr(Cx &cx, const Ops &body);
    void eval(Cx &cx, Env &env, Stack &stack);
    void mark();
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };
}

#endif
