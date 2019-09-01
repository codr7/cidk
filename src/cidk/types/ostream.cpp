#include <iostream>

#include "cidk/types/ostream.hpp"
#include "cidk/val.hpp"

namespace cidk {
  OStreamType::OStreamType(Cx &cx,
                           const Pos &pos,
                           Env &env,
                           const Sym *id,
                           const vector<Type *> &parents):
    TValType<OStream *>(cx, pos, env, id, parents) {}

  void OStreamType::dump(const Val &val, ostream &out) const {
    out << "OStream(" << &val.as_ostream->imp << ')';
  }

  bool OStreamType::is(const Val &x, const Val &y) const {
    return x.as_ostream == y.as_ostream;
  }

  void OStreamType::mark_refs(const Val &val) const {
    val.as_ostream->ref_mark = true;
  }

  void OStreamType::set(Val &dst, OStream *val) const { dst.as_ostream = val; }

  void OStreamType::sweep(const Pos &pos, Val &val) {
    val.as_ostream->sweep(cx, pos);
  }
}
