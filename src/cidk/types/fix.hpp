#ifndef CIDK_TYPE_FIX_HPP
#define CIDK_TYPE_FIX_HPP

#include "cidk/fix.hpp"
#include "cidk/val_type.hpp"

namespace cidk {  
  struct FixType: TValType<Fix> {
    FixType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);
    
    virtual int cmp(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void dump(const Val &val, ostream &out) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void set(Val &dst, Fix val) const override;

    virtual bool get_bool(const Val &val) const override;
  };
}

#endif
