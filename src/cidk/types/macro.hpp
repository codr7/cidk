#ifndef CIDK_TYPE_MACRO_HPP
#define CIDK_TYPE_MACRO_HPP

#include "cidk/macro.hpp"
#include "cidk/pool.hpp"
#include "cidk/val_type.hpp"

namespace cidk {  
  struct Macro;
  
  struct MacroType: TValType<Macro *> {
    Pool<Macro> pool;
    
    MacroType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual void dump(const Pos &Pos, const Val &val, ostream &out) const override;
    virtual void dup(Val &dst, const Val &src) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void mark_refs(const Val &val) const override;
    virtual void set(const Pos &pos, Val &dst, Macro *val) const override;
    virtual void sweep(const Pos &pos, Val &val) override;
  };
}

#endif
