#ifndef CIDK_TYPE_CHAR_HPP
#define CIDK_TYPE_CHAR_HPP

#include "cidk/val_type.hpp"

namespace cidk {  
  using Char = wchar_t;

  struct CharType: TValType<Char> {
    CharType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);
    
    virtual void cp(Val &dst, const Val &src) const override;
    virtual void dump(const Val &val, ostream &out) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void set(const Pos &pos, Val &dst, Char val) const override;

    virtual bool get_bool(const Val &val) const override;
  };
}

#endif
