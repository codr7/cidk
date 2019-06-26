#ifndef CIDK_TYPE_FUN_HPP
#define CIDK_TYPE_FUN_HPP

#include "cidk/fun.hpp"
#include "cidk/pool.hpp"
#include "cidk/val_type.hpp"

namespace cidk {  
  struct Fun;
  
  struct FunType: ValTType<Fun *> {
    Pool<Fun> pool;
    
    FunType(Cx &cx, const Pos &pos, string id);

    virtual void call(const Pos &pos, const Val &val) const override;
    virtual void dup(const Pos &pos, Val &dst, const Val &src) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void mark_refs(const Pos &pos, const Val &val) override;
    virtual void set(const Pos &pos, Val &dst, Fun *val) const override;
    virtual void sweep(const Pos &pos, Val &val) override;
  };
}

#endif
