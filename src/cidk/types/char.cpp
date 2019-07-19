#include "cidk/cx.hpp"
#include "cidk/types/char.hpp"
#include "cidk/types/str.hpp"
#include "cidk/val.hpp"

namespace cidk {
  CharType::CharType(Cx &cx,
                     const Pos &pos,
                     const Sym *id,
                     const vector<Type *> &parents):
    TValType<Char>(cx, pos, id, parents) {}

  void CharType::cp(Val &dst, const Val &src) const { dst.as_char = src.as_char; }

  void CharType::dump(const Val &val, ostream &out) const {
    wchar_t c(val.as_char);

    switch (c) {
    case L'"':
      out << "\\\"";
      break;
    default:
      wchar_t in[2] {val.as_char, 0};
      out << cx.str_type.conv.to_bytes(in);
    }
  }

  bool CharType::is(const Val &x, const Val &y) const {
    return x.as_char == y.as_char;
  }

  void CharType::set(const Pos &pos, Val &dst, Char val) const { dst.as_char = val; }

  bool CharType::get_bool(const Val &val) const { return val.as_char > 0; }
}
