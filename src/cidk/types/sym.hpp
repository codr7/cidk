#ifndef CIDK_TYPE_SYM_HPP
#define CIDK_TYPE_SYM_HPP

#include "cidk/sym.hpp"
#include "cidk/val_type.hpp"

namespace cidk {  
  struct SymType: TValType<const Sym *> {
    SymType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual void dump(const Pos &Pos, const Val &val, ostream &out) const override;
    virtual void dup(Val &dst, const Val &src) const override;

    virtual void eval(const Pos &pos,
                      const Val &val,
                      Env &env,
                      Stack &stack) const override;
    
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void set(const Pos &pos, Val &dst, const Sym *val) const override;
  };
}

#endif
