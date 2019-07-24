#ifndef CIDK_TYPE_SEXPR_HPP
#define CIDK_TYPE_SEXPR_HPP

#include "cidk/types/expr.hpp"

namespace cidk {
  struct SexprType: ExprType {
    SexprType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);
    
    virtual void eval(Cx &cx,
                      const Pos &pos,
                      const Val &val,
                      Env &env,
                      Stack &stack) const override;
  };
}

#endif
