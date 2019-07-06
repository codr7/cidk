#include "cidk/cx.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  Ref::Ref(Cx &cx):
    cx(cx), ref_it(cx.refs.insert(cx.refs.end(), this)), is_marked(true) { }
}
