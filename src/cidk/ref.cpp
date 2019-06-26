#include "cidk/cx.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  Ref::Ref(Cx &cx): cx(cx), ref_state(RefState::mark) {
    cx.refs.push_back(this);
  }
}
