#ifndef CIDK_VAL_TYPE_HPP
#define CIDK_VAL_TYPE_HPP

#include "cidk/e.hpp"
#include "cidk/ops.hpp"
#include "cidk/opts.hpp"
#include "cidk/reg.hpp"
#include "cidk/sym.hpp"
#include "cidk/type.hpp"

namespace cidk {
  struct ConstType;
  struct Env;
  struct Val;

  struct ValType: Type {
    bool is_const;
    ConstType *_const_type;
    
    ValType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    ConstType &const_type(const Pos &pos);

    virtual void clone(const Pos &pos, Val &dst, const Val &src) const {}

    virtual int cmp(const Pos &pos, const Val &x, const Val &y) const {
      throw ESys(pos, "Not comparable: ", id);
    }
    
    virtual void compile(const Pos &pos, Val &val, Env &env, Opts &opts) const {}
    virtual void dump(const Val &val, ostream &out) const = 0;

    virtual bool eq(const Pos &pos, const Val &x, const Val &y) const {
      return is(x, y);
    }

    virtual void eval(const Pos &pos, const Val &val, Env &env, Reg *regs) const;

    virtual bool find_op(const Val &val,
                         function<bool (Ops &, OpIter &)> pred) const {
      return false;
    }

    virtual bool is(const Val &x, const Val &y) const = 0;
    virtual void mark_refs(const Val &val) const {};
    virtual void splat(const Pos &pos, const Val &val, int max) const;
    
    using Type::sweep;
    virtual void sweep(const Pos &pos, Val &val) {}

    virtual bool get_bool(const Val &val) const { return true; }
  };
  
  template <typename ValT>
  struct TValType: ValType {
    TValType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents):
      ValType(cx, pos, id, parents) {}
    
    virtual void set(Val &dst, ValT val) const = 0;
  };
}

#endif
