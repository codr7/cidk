#ifndef CIDK_TYPE_EXPR_HPP
#define CIDK_TYPE_EXPR_HPP

#include "cidk/expr.hpp"
#include "cidk/pool.hpp"
#include "cidk/val_type.hpp"

namespace cidk {
  struct ExprType: TValType<Expr *> {
    Pool<Expr> pool;

    ExprType(Cx &cx, const Pos &pos, const Sym *id);
    
    virtual void dump(const Pos &Pos, const Val &val, ostream &out) const override;
    virtual void dup(const Pos &pos, Val &dst, const Val &src) const override;
    virtual void eval(const Val &val, Env &env) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void mark_refs(const Pos &pos, const Val &val) override;
    virtual void set(const Pos &pos, Val &dst, Expr *val) const override;
    virtual void sweep(const Pos &pos, Val &val) override;
  };
}

#endif
