#ifndef CIDK_TYPE_NIL_HPP
#define CIDK_TYPE_NIL_HPP

#include "cidk/val_type.hpp"

namespace cidk {  
  struct NilType: ValType {
    NilType(Cx &cx,
            const Pos &pos,
            Env &env,
            const Sym *id,
            const vector<Type *> &parents):
      ValType(cx, pos, env, id, parents) {
      nil_type = this;
    }

    virtual void dump(const Val &val, ostream &out) const override { out << '_'; }
    
    virtual bool is(const Val &x, const Val &y) const override { return true; }

    virtual bool get_bool(const Val &val) const override { return false; }
  };
}

#endif
