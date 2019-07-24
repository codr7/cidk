#include "cidk/types/sexpr.hpp"
#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk {
  SexprType::SexprType(Cx &cx,
                 const Pos &pos,
                 const Sym *id,
                 const vector<Type *> &parents):
    ExprType(cx, pos, id, parents) {}
  
  void SexprType::eval(Cx &cx,
                    const Pos &pos,
                    const Val &val,
                    Env &env,
                    Stack &stack) const {
    val.as_expr->body.front().eval(cx, env, stack);
  }
}
