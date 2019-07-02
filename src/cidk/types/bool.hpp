#ifndef CIDK_TYPE_BOOL_HPP
#define CIDK_TYPE_BOOL_HPP

#include "cidk/val_type.hpp"

namespace cidk {  
  struct BoolType: TValType<bool> {
    BoolType(Cx &cx, const Pos &pos, const Sym *id);
    
    virtual void dump(const Pos &Pos, const Val &val, ostream &out) const override;
    virtual void dup(Val &dst, const Val &src) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void set(const Pos &pos, Val &dst, bool val) const override;

    virtual bool Bool(const Pos &pos, const Val &val) const override;
  };
}

#endif
