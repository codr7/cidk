#include "cidk/cx.hpp"
#include "cidk/ostream.hpp"

namespace cidk {
  OStream::OStream (Cx &cx, ostream &imp): Ref(cx), imp(imp) {}

  void OStream::sweep(Cx &cx, const Pos &pos) { cx.ostream_type.pool.put(this); }
}
