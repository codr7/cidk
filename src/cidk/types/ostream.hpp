#ifndef CIDK_TYPE_OSTREAM_HPP
#define CIDK_TYPE_OSTREAM_HPP

#include "cidk/ostream.hpp"
#include "cidk/pool.hpp"
#include "cidk/val_type.hpp"

namespace cidk {  
  struct OStreamType: TValType<OStream *> {
    Pool<OStream> pool;
    
    OStreamType(Cx &cx, const Pos &pos, const Sym *id, const vector<Type *> &parents);

    virtual void dump(const Pos &Pos, const Val &val, ostream &out) const override;
    virtual void dup(Val &dst, const Val &src) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void mark_refs(const Pos &pos, const Val &val) override;
    virtual void set(const Pos &pos, Val &dst, OStream *val) const override;
    virtual void sweep(const Pos &pos, Val &val) override;
  };
}

#endif
