#ifndef CIDK_TYPE_CONST_HPP
#define CIDK_TYPE_CONST_HPP

#include "cidk/val_type.hpp"

namespace cidk {
  struct ConstType: ValType {
    ValType &val_type;
    
    ConstType(Cx &cx, const Pos &pos, ValType &val_type);

    virtual void clone(const Pos &pos, Val &dst, const Val &src) const override;

    virtual void compile(const Pos &pos,
                         Val &val,
                         Env &env,
                         Opts &opts) const override {
      val_type.compile(pos, val, env, opts);
    }

    virtual void cp(Val &dst, const Val &src) const override {
      val_type.cp(dst, src);
    }

    virtual void dump(const Val &val, ostream &out) const override;

    virtual bool is(const Val &x, const Val &y) const override {
      return val_type.is(x, y);
    }

    virtual void mark_refs(const Val &val) const override { val_type.mark_refs(val); }

    virtual bool get_bool(const Val &val) const override {
      return val_type.get_bool(val);
    }
  };
}

#endif
