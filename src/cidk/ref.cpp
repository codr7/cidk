#include "cidk/cx.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  Ref::Ref(Cx &cx): ref_state(RefState::keep), ref_marked(false) {
    cx.refs.push(*this);
  }
}
