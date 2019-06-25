#ifndef CIDK_TYPE_META_HPP
#define CIDK_TYPE_META_HPP

#include "cidk/val_type.hpp"

namespace cidk {  
  struct MetaType: ValTType<Type *> {
    MetaType(Cx &cx, const Pos &pos, const string &id);

    virtual void dup(const Pos &pos, Val &dst, const Val &src) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void set(const Pos &pos, Val &dst, Type *val) const override;
    virtual void sweep(const Pos &pos, Val &val) override;
  };
}

#endif
