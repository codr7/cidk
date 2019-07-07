#ifndef CIDK_TYPE_LIST_HPP
#define CIDK_TYPE_LIST_HPP

#include "cidk/list.hpp"
#include "cidk/pool.hpp"
#include "cidk/val_type.hpp"

namespace cidk {
  struct ListType: TValType<List *> {
    Pool<List> pool;

    ListType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual void clone(const Pos &pos, Val &dst, const Val &src) const override;
    virtual void dump(const Pos &Pos, const Val &val, ostream &out) const override;
    virtual void dup(Val &dst, const Val &src) const override;
    virtual bool eq(const Pos &pos, const Val &x, const Val &y) const override;

    virtual void eval(const Pos &pos,
                      const Val &val,
                      Env &env,
                      Stack &stack) const override;

    virtual void get_ids(const Val &val, IdSet &out) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void mark_refs(const Pos &pos, const Val &val) const override;
    virtual void set(const Pos &pos, Val &dst, List *val) const override;

    virtual void splat(const Pos &pos,
                       const Val &val,
                       Env &env, Stack
                       &stack) const override;
    
    virtual void sweep(const Pos &pos, Val &val) override;

    virtual bool Bool(const Pos &pos, const Val &val) const override;
  };
}

#endif
