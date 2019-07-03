#ifndef CIDK_VAL_TYPE_HPP
#define CIDK_VAL_TYPE_HPP

#include "cidk/ops.hpp"
#include "cidk/stack.hpp"
#include "cidk/type.hpp"

namespace cidk {
  struct ConstType;
  struct Val;

  struct ValType: Type {
    bool is_const;
    ConstType *_const_type;
    
    ValType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    ConstType &const_type(const Pos &pos);
    
    virtual void call(const Pos &pos, const Val &val, Env &env, Stack &stack) const;
    virtual void clone(const Pos &pos, Val &dst, const Val &src) const;
    virtual void dump(const Pos &Pos, const Val &val, ostream &out) const = 0;
    virtual void dup(Val &dst, const Val &src) const = 0;
    virtual bool eq(const Pos &pos, const Val &x, const Val &y) const;
    virtual void eval(const Pos &pos, const Val &val, Env &env, Stack &stack) const;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const = 0;
    virtual void mark_refs(const Pos &pos, const Val &val);
    virtual void splat(const Pos &pos, Val &val, Env &env, Stack &stack);
    virtual void sweep(const Pos &pos, Val &val);

    virtual bool Bool(const Pos &pos, const Val &val) const;
  };
  
  template <typename ValT>
  struct TValType: ValType {
    TValType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents):
      ValType(cx, pos, id, parents) { }
    
    virtual void set(const Pos &pos, Val &dst, ValT val) const = 0;
  };
}

#endif
