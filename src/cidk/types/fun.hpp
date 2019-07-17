#ifndef CIDK_TYPE_FUN_HPP
#define CIDK_TYPE_FUN_HPP

#include "cidk/fun.hpp"
#include "cidk/pool.hpp"
#include "cidk/val_type.hpp"

namespace cidk {  
  struct Fun;
  
  struct FunType: TValType<Fun *> {
    Pool<Fun> pool;
    
    FunType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual void cp(Val &dst, const Val &src) const override;
    virtual void dump(const Val &val, ostream &out) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void mark_refs(const Val &val) const override;
    virtual void set(const Pos &pos, Val &dst, Fun *val) const override;
    virtual void sweep(const Pos &pos, Val &val) override;

    virtual Env &get_env(Val &val) const override;
  };
}

#endif
