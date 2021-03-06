#ifndef CIDK_TYPE_INT_HPP
#define CIDK_TYPE_INT_HPP

#include "cidk/val_type.hpp"

namespace cidk {  
  struct IntType: TValType<Int> {
    IntType(Cx &cx,
            const Pos &pos,
            Env &env,
            const Sym *id,
            const vector<Type *> &parents);
    
    virtual int cmp(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void dump(const Val &val, ostream &out) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void set(Val &dst, Int val) const override;

    virtual bool get_bool(const Val &val) const override;
  };
}

#endif
