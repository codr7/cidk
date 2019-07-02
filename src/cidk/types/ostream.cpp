#include <iostream>

#include "cidk/types/ostream.hpp"
#include "cidk/val.hpp"

namespace cidk {
  OStreamType::OStreamType(Cx &cx, const Pos &pos, const Sym *id):
    TValType<OStream *>(cx, pos, id) { }

  void OStreamType::dump(const Pos &Pos, const Val &val, ostream &out) const {
    out << "OStream(" << &val.as_ostream->imp << ')';
  }

  void OStreamType::dup(Val &dst, const Val &src) const {
    dst.as_ostream = src.as_ostream;
  }

  bool OStreamType::is(const Pos &pos, const Val &x, const Val &y) const {
    return x.as_ostream == y.as_ostream;
  }

  void OStreamType::mark_refs(const Pos &pos, const Val &val) {
    val.as_ostream->ref_state = RefState::mark;
  }

  void OStreamType::set(const Pos &pos, Val &dst, OStream *val) const {
    dst.as_ostream = val;
  }

  void OStreamType::sweep(const Pos &pos, Val &val) {
    val.as_ostream->sweep(pos);
  }
}
