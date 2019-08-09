#ifndef CIDK_VAL_TYPE_HPP
#define CIDK_VAL_TYPE_HPP

#include "cidk/ops.hpp"
#include "cidk/opts.hpp"
#include "cidk/reg.hpp"
#include "cidk/sym.hpp"
#include "cidk/type.hpp"

namespace cidk {
  struct ConstType;
  struct Val;

  struct ValType: Type {
    bool is_const;
    ConstType *_const_type;
    
    ValType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    ConstType &const_type(const Pos &pos);

    virtual void cp(Val &dst, const Val &src) const = 0;

    virtual void clone(const Pos &pos, Val &dst, const Val &src) const {
      cp(dst, src);
    }
    
    virtual void compile(Cx &cx,
                         const Pos &pos,
                         Val &val,
                         Env &env,
                         Opts &opts) const;

    virtual void dump(const Val &val, ostream &out) const = 0;
    virtual bool eq(const Pos &pos, const Val &x, const Val &y) const;

    virtual void eval(Cx &cx,
                      const Pos &pos,
                      const Val &val,
                      Env &env,
                      Reg *regs) const;

    virtual bool is(const Val &x, const Val &y) const = 0;
    virtual void mark_refs(const Val &val) const;
    virtual void splat(const Pos &pos, const Val &val, Env &env) const;
    
    using Type::sweep;
    virtual void sweep(const Pos &pos, Val &val);

    virtual bool get_bool(const Val &val) const;
    virtual Env &get_env(Val &val) const;
  };
  
  template <typename ValT>
  struct TValType: ValType {
    TValType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents):
      ValType(cx, pos, id, parents) {}
    
    virtual void set(Val &dst, ValT val) const = 0;
  };
}

#endif
