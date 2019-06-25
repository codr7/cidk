#ifndef CIDK_TYPE_BYTE_HPP
#define CIDK_TYPE_BYTE_HPP

#include <cstdint>

#include "cidk/val_type.hpp"

namespace cidk {  
  using Byte = uint8_t;

  struct ByteType: ValTType<Byte> {
    ByteType(Cx &cx, const Pos &pos, const string &id);
    
    virtual void dup(const Pos &pos, Val &dst, const Val &src) const override;
    virtual bool is(const Pos &pos, const Val &x, const Val &y) const override;
    virtual void set(const Pos &pos, Val &dst, Byte val) const override;
  };
}

#endif
