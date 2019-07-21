#ifndef CIDK_TYPE_SYM_HPP
#define CIDK_TYPE_SYM_HPP

#include "cidk/sym.hpp"
#include "cidk/val_type.hpp"

namespace cidk {  
  struct SymType: TValType<const Sym *> {
    SymType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual void compile(Cx &cx,
                         const Pos &pos,
                         Val &val,
                         Env &env,
                         Stack &stack,
                         Opts *opts) const override;

    virtual void cp(Val &dst, const Val &src) const override;
    virtual void dump(const Val &val, ostream &out) const override;

    virtual void eval(Cx &cx,
                      const Pos &pos,
                      const Val &val,
                      Env &env,
                      Stack &stack) const override;
    
    virtual void get_ids(const Val &val, IdSet &out) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void set(Val &dst, const Sym *val) const override;
  };
}

#endif
