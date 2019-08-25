#ifndef CIDK_TYPE_REG_HPP
#define CIDK_TYPE_REG_HPP

#include "cidk/val_type.hpp"

namespace cidk {  
  struct RegType: TValType<size_t> {
    RegType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual void dump(const Val &val, ostream &out) const override;

    virtual void eval(const Pos &pos,
                      const Val &val,
                      Env &env,
                      Reg *regs) const override;
    
    virtual bool is(const Val &x, const Val &y) const override;

    virtual void set(Val &dst, size_t val) const override;
  };
}

#endif
