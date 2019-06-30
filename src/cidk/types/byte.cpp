#include <iostream>
#include "cidk/types/byte.hpp"
#include "cidk/val.hpp"

namespace cidk {
  ByteType::ByteType(Cx &cx, const Pos &pos, const string &id):
    ValTType<Byte>(cx, pos, id) { }

  void ByteType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << val.as_byte;
  }

  void ByteType::dup(const Pos &pos, Val &dst, const Val &src) const {
    dst.as_byte = src.as_byte;
  }

  bool ByteType::is(const Pos &pos, const Val &x, const Val &y) const {
    return x.as_byte == y.as_byte;
  }

  void ByteType::set(const Pos &pos, Val &dst, Byte val) const {
    dst.as_byte = val;
  }

  bool ByteType::Bool(const Pos &pos, const Val &val) const {
    return val.as_byte > 0;
  }
}
