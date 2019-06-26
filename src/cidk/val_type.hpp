#ifndef CIDK_VAL_TYPE_HPP
#define CIDK_VAL_TYPE_HPP

#include "cidk/type.hpp"
#include "cidk/ops.hpp"

namespace cidk {
  struct Val;

  struct ValType: Type {
    ValType(Cx &cx, const Pos &pos, const string &id);
    virtual void init() override;
    
    virtual void call(const Pos &pos, const Val &val) const;
    virtual void compile(const Pos &pos, const Val &val, Ops &out) const;
    virtual void dup(const Pos &pos, Val &dst, const Val &src) const = 0;
    virtual bool eq(const Pos &pos, const Val &x, const Val &y) const;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const = 0;
    virtual void mark_refs(const Pos &pos, const Val &val);
    virtual void splat(const Pos &pos, Val &val);
    virtual void sweep(const Pos &pos, Val &val);
  };
  
  template <typename ValT>
  struct ValTType: ValType {
    ValTType(Cx &cx, const Pos &pos, const string &id): ValType(cx, pos, id) { }
    virtual void set(const Pos &pos, Val &dst, ValT val) const = 0;
  };
}

#endif
