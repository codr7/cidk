#ifndef CIDK_TYPE_META_HPP
#define CIDK_TYPE_META_HPP

#include "cidk/val_type.hpp"

namespace cidk {  
  struct MetaType: TValType<Type *> {
    MetaType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual void dump(const Val &val, ostream &out) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void mark_refs(const Val &val) const override;
    virtual void set(Val &dst, Type *val) const override;
    virtual void sweep(const Pos &pos, Val &val) override;
  };
}

#endif
