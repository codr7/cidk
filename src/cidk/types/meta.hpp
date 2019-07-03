#ifndef CIDK_TYPE_META_HPP
#define CIDK_TYPE_META_HPP

#include "cidk/val_type.hpp"

namespace cidk {  
  struct MetaType: TValType<Type *> {
    MetaType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual void dump(const Pos &Pos, const Val &val, ostream &out) const override;
    virtual void dup(Val &dst, const Val &src) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void mark_refs(const Pos &pos, const Val &val) override;
    virtual void set(const Pos &pos, Val &dst, Type *val) const override;
    virtual void sweep(const Pos &pos, Val &val) override;
  };
}

#endif
