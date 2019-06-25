#include "cidk/cx.hpp"
#include "cidk/ref.hpp"

namespace cidk {
  Ref::Ref(Cx &cx):
    cx(cx), it(cx.refs.insert(cx.refs.end(), this)), state(State::_) { }

  Ref::~Ref() {
    cx.refs.erase(it);
  }
}
