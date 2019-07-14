#include "cidk/cx.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  Ref::Ref(Cx &cx): ref_mark(true) { cx.refs.push(*this); }
}
