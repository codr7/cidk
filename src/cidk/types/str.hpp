#ifndef CIDK_TYPE_STR_HPP
#define CIDK_TYPE_STR_HPP

#include <codecvt>
#include <locale>

#include "cidk/str.hpp"
#include "cidk/pool.hpp"
#include "cidk/types/char.hpp"

namespace cidk {
  struct StrType: TValType<Str *> {
    Pool<Str> pool;
    wstring_convert<codecvt_utf8<Char>, Char> conv;
    
    StrType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual void clone(const Pos &pos, Val &dst, const Val &src) const override;
    virtual void cp(Val &dst, const Val &src) const override;
    virtual void dump(const Val &val, ostream &out) const override;
    virtual bool eq(const Pos &pos, const Val &x, const Val &y) const override;

    virtual bool is(const Val &x, const Val &y) const override;
    virtual void mark_refs(const Val &val) const override;
    virtual void set(Val &dst, Str *val) const override;

    virtual void splat(const Pos &pos,
                       const Val &val,
                       Env &env) const override;
    
    virtual void sweep(const Pos &pos, Val &val) override;

    virtual bool get_bool(const Val &val) const override;
  };
}

#endif
