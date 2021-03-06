#ifndef CIDK_TYPE_EXPR_HPP
#define CIDK_TYPE_EXPR_HPP

#include "cidk/expr.hpp"
#include "cidk/pool.hpp"
#include "cidk/val_type.hpp"

namespace cidk {
  struct ExprType: TValType<Expr *> {
    Pool<Expr> pool;

    ExprType(Cx &cx,
             const Pos &pos,
             Env &env,
             const Sym *id,
             const vector<Type *> &parents);
    
    virtual void compile(const Pos &pos,
                         Val &val,
                         Env &env,
                         Opts &opts) const override;

    virtual void dump(const Val &val, ostream &out) const override;

    virtual bool eval(const Pos &pos,
                      const Val &val,
                      Env &env,
                      Reg *regs) const override;

    virtual bool find_op(const Val &val,
                         function<bool (Ops &, OpIter &)> pred) const override;

    virtual bool is(const Val &x, const Val &y) const override;
    virtual void mark_refs(const Val &val) const override;
    virtual void set(Val &dst, Expr *val) const override;
    virtual void sweep(const Pos &pos, Val &val) override;
  };
}

#endif
