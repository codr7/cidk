#ifndef CIDK_TYPE_POP_HPP
#define CIDK_TYPE_POP_HPP

#include "cidk/val_type.hpp"

namespace cidk {  
  struct PopType: ValType {
    PopType(Cx &cx,
            const Pos &pos,
            Env &env,
            const Sym *id,
            const vector<Type *> &parents):
      ValType(cx, pos, env, id, parents) {}

    virtual void dump(const Val &val, ostream &out) const override { out << '$'; }

    virtual bool eval(const Pos &pos,
                      const Val &val,
                      Env &env,
                      Reg *regs) const override { return true; }
    
    virtual bool is(const Val &x, const Val &y) const override { return true; }
  };
}

#endif
