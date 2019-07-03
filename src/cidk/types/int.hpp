#ifndef CIDK_TYPE_INT_HPP
#define CIDK_TYPE_INT_HPP

#include <cstdint>

#include "cidk/val_type.hpp"

namespace cidk {  
  using Int = int64_t;

  struct IntType: TValType<Int> {
    IntType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);
    
    virtual void dump(const Pos &Pos, const Val &val, ostream &out) const override;
    virtual void dup(Val &dst, const Val &src) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void set(const Pos &pos, Val &dst, Int val) const override;
    virtual void splat(const Pos &pos, Val &val) override;

    virtual bool Bool(const Pos &pos, const Val &val) const override;
  };
}

#endif
