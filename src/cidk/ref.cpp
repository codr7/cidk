#include "cidk/cx.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  Ref::Ref(Cx &cx, SweepImp sweep):
    cx(cx), it(cx.refs.insert(cx.refs.end(), this)), sweep(sweep), marks(0) { }

  Ref::~Ref() {
    cx.refs.erase(it);
  }
}
