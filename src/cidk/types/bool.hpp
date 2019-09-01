#ifndef CIDK_TYPE_BOOL_HPP
#define CIDK_TYPE_BOOL_HPP

#include "cidk/val_type.hpp"

namespace cidk {  
  struct BoolType: TValType<bool> {
    BoolType(Cx &cx,
             const Pos &pos,
             Env &env,
             const Sym *id,
             const vector<Type *> &parents);
    
    virtual void dump(const Val &val, ostream &out) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void set(Val &dst, bool val) const override;

    virtual bool get_bool(const Val &val) const override;
  };
}

#endif
