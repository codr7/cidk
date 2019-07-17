#ifndef CIDK_TYPE_OSTREAM_HPP
#define CIDK_TYPE_OSTREAM_HPP

#include "cidk/ostream.hpp"
#include "cidk/pool.hpp"
#include "cidk/val_type.hpp"

namespace cidk {  
  struct OStreamType: TValType<OStream *> {
    Pool<OStream> pool;
    
    OStreamType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual void cp(Val &dst, const Val &src) const override;
    virtual void dump(const Val &val, ostream &out) const override;
    virtual bool is(const Val &x, const Val &y) const override;
    virtual void mark_refs(const Val &val) const override;
    virtual void set(const Pos &pos, Val &dst, OStream *val) const override;
    virtual void sweep(const Pos &pos, Val &val) override;
  };
}

#endif
