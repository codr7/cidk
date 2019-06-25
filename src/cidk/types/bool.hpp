#ifndef CIDK_TYPE_BOOL_HPP
#define CIDK_TYPE_BOOL_HPP

#include "cidk/val_type.hpp"

namespace cidk {  
  struct BoolType: ValTType<bool> {
    BoolType(Cx &cx, const Pos &pos, const string &id);
    
    virtual void dup(const Pos &pos, Val &dst, const Val &src) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void set(const Pos &pos, Val &dst, bool val) const override;
  };
}

#endif
