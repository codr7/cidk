#ifndef CIDK_TYPE_INT_HPP
#define CIDK_TYPE_INT_HPP

#include <cstdint>

#include "cidk/val_type.hpp"

namespace cidk {  
  using Int = int64_t;

  struct IntType: TValType<Int> {
    IntType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);
    
    virtual void cp(Val &dst, const Val &src) const override;
    virtual void dump(const Val &val, ostream &out) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void set(Val &dst, Int val) const override;

    virtual void splat(const Pos &pos,
                       const Val &val,
                       Env &env,
                       Stack &stack) const override;

    virtual bool get_bool(const Val &val) const override;
  };
}

#endif
