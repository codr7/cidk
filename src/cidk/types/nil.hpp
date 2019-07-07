#ifndef CIDK_TYPE_NIL_HPP
#define CIDK_TYPE_NIL_HPP

#include "cidk/val_type.hpp"

namespace cidk {  
  struct NilType: ValType {
    NilType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual void dump(const Pos &Pos, const Val &val, ostream &out) const override;
    virtual void dup(Val &dst, const Val &src) const override;
    virtual bool is(const Val &x, const Val &y) const override;

    virtual bool Bool(const Pos &pos, const Val &val) const override;
  };
}

#endif
