#ifndef CIDK_TYPE_PAIR_HPP
#define CIDK_TYPE_PAIR_HPP

#include "cidk/pair.hpp"
#include "cidk/pool.hpp"
#include "cidk/val_type.hpp"

namespace cidk {
  struct PairType: TValType<Pair *> {
    Pool<Pair> pool;

    PairType(Cx &cx,
             const Pos &pos,
             Env &env,
             const Sym *id,
             const vector<Type *> &parents);

    virtual void clone(const Pos &pos, Val &dst, const Val &src) const override;

    virtual void compile(const Pos &pos,
                         Val &val,
                         Env &env,
                         Opts &opts) const override;

    virtual void dump(const Val &val, ostream &out) const override;
    virtual bool eq(const Pos &pos, const Val &x, const Val &y) const override;

    virtual bool eval(const Pos &pos,
                      const Val &val,
                      Env &env,
                      Reg *regs) const override;
    
    virtual bool find_op(const Val &val,
                         function<bool (Ops &, OpIter &)> pred) const override;
    
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void mark_refs(const Val &val) const override;
    virtual void set(Val &dst, Pair *val) const override;
    virtual void splat(const Pos &pos, const Val &val, int max) const override;
    virtual void sweep(const Pos &pos, Val &val) override;
  };
}

#endif
