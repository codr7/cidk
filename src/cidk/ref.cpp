#include "cidk/cx.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  Ref::Ref(Cx &cx): is_marked(true) { cx.refs.push(*this); }
}
