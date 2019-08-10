#ifndef CIDK_TYPE_CONST_HPP
#define CIDK_TYPE_CONST_HPP

#include "cidk/val_type.hpp"

namespace cidk {
  struct ConstType: ValType {
    ValType &val_type;
    
    ConstType(Cx &cx, const Pos &pos, ValType &val_type);

    virtual void clone(const Pos &pos, Val &dst, const Val &src) const override;
    virtual void cp(Val &dst, const Val &src) const override;

    virtual void compile(const Pos &pos,
                         Val &val,
                         Env &env,
                         Opts &opts) const override;

    virtual void dump(const Val &val, ostream &out) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void mark_refs(const Val &val) const override;
    virtual bool get_bool(const Val &val) const override;
  };
}

#endif
