#ifndef CIDK_TYPE_ENV_HPP
#define CIDK_TYPE_ENV_HPP

#include "cidk/env.hpp"
#include "cidk/pool.hpp"
#include "cidk/val_type.hpp"

namespace cidk {
  struct EnvType: TValType<Env *> {
    Pool<Env> pool;

    EnvType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual void clone(const Pos &pos, Val &dst, const Val &src) const override;
    virtual void dump(const Pos &Pos, const Val &val, ostream &out) const override;
    virtual void dup(Val &dst, const Val &src) const override;
    virtual bool eq(const Pos &pos, const Val &x, const Val &y) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void mark_refs(const Pos &pos, const Val &val) override;
    virtual void set(const Pos &pos, Val &dst, Env *val) const override;
    virtual void sweep(const Pos &pos, Val &val) override;

    virtual bool Bool(const Pos &pos, const Val &val) const override;
  };
}

#endif
