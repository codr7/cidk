#ifndef CIDK_TYPE_BYTE_HPP
#define CIDK_TYPE_BYTE_HPP

#include <cstdint>

#include "cidk/val_type.hpp"

namespace cidk {  
  using Byte = uint8_t;

  struct ByteType: TValType<Byte> {
    ByteType(Cx &cx, const Pos &pos, const Sym *id);
    
    virtual void dump(const Pos &Pos, const Val &val, ostream &out) const override;
    virtual void dup(const Pos &pos, Val &dst, const Val &src) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void set(const Pos &pos, Val &dst, Byte val) const override;

    virtual bool Bool(const Pos &pos, const Val &val) const override;
  };
}

#endif
