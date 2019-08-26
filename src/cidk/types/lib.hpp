#ifndef CIDK_TYPE_LIB_HPP
#define CIDK_TYPE_LIB_HPP

#include "cidk/lib.hpp"
#include "cidk/pool.hpp"
#include "cidk/val_type.hpp"

namespace cidk {  
  struct Lib;
  
  struct LibType: TValType<Lib *> {
    Pool<Lib> pool;
    
    LibType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual int cmp(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void dump(const Val &val, ostream &out) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void mark_refs(const Val &val) const override;
    virtual void set(Val &dst, Lib *val) const override;
    virtual void sweep(const Pos &pos, Val &val) override;
  };
}

#endif
