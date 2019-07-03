#ifndef CIDK_TYPE_CONST_HPP
#define CIDK_TYPE_CONST_HPP

#include "cidk/val_type.hpp"

namespace cidk {
  struct ConstType: ValType {
    ValType &val_type;
    
    ConstType(Cx &cx, const Pos &pos, ValType &val_type);

    virtual void clone(const Pos &pos, Val &dst, const Val &src) const override;
    virtual void dump(const Pos &Pos, const Val &val, ostream &out) const override;
    virtual void dup(Val &dst, const Val &src) const override;
    virtual void eval(const Pos &pos, const Val &val, Env &env, Stack &stack) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    void mark_refs(const Pos &pos, const Val &val);

    virtual bool Bool(const Pos &pos, const Val &val) const override;
  };
}

#endif
