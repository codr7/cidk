#ifndef CIDK_TYPE_LIST_HPP
#define CIDK_TYPE_LIST_HPP

#include "cidk/list.hpp"
#include "cidk/pool.hpp"
#include "cidk/val_type.hpp"

namespace cidk {
  struct ListType: ValTType<List *> {
    Pool<List> pool;

    ListType(Cx &cx, const Pos &pos, const string &id);
    
    virtual void dup(const Pos &pos, Val &dst, const Val &src) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void mark_refs(const Pos &pos, const Val &val) override;
    virtual void set(const Pos &pos, Val &dst, List *val) const override;
    virtual void splat(const Pos &pos, Val &val) override;
    virtual void sweep(const Pos &pos, Val &val) override;
  };
}

#endif
