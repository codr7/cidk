#ifndef CIDK_TYPE_INT_HPP
#define CIDK_TYPE_INT_HPP

#include <cstdint>

#include "cidk/val_type.hpp"

namespace cidk {  
  using Int = int64_t;

  struct IntType: ValTType<Int> {
    IntType(Cx &cx, const Pos &pos, const string &id);
    
    virtual void dup(const Pos &pos, Val &dst, const Val &src) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void set(const Pos &pos, Val &dst, Int val) const override;
    virtual void splat(const Pos &pos, Val &val) override;
  };
}

#endif
