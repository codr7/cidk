#ifndef CIDK_TYPE_E_HPP
#define CIDK_TYPE_E_HPP

#include "cidk/e_user.hpp"
#include "cidk/pool.hpp"
#include "cidk/val_type.hpp"

namespace cidk {  
  struct EType: TValType<EUser *> {
    Pool<EUser> pool;
    
    EType(Cx &cx,
          const Pos &pos,
          Env &env,
          const Sym *id,
          const vector<Type *> &parents);

    virtual int cmp(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void dump(const Val &val, ostream &out) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void mark_refs(const Val &val) const override;
    virtual void set(Val &dst, EUser *val) const override;
    virtual void sweep(const Pos &pos, Val &val) override;
  };
}

#endif
