#ifndef CIDK_EXPR_HPP
#define CIDK_EXPR_HPP

#include "cidk/ops.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  struct Val;
  
  struct Expr: Ref {
    Ops body;
    
    Expr(Cx &cx);
    virtual void sweep(const Pos &pos) override;
  };
}

#endif
