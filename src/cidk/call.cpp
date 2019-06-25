#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"

namespace cidk {  
  Call::Call(Cx &cx, const Pos &pos, Fun &target):
    cx(cx), pos(pos), prev(cx.call), target(target) {
    cx.call = this;
  }

  Call::~Call() noexcept(false) {
    if (cx.call != this) {
      throw SysE(pos, "Calls ended out of order");
    }

    cx.call = prev;
  }

  void Call::eval() {
    auto imp(target.imp);
    
    if (imp) {
      imp(*this);
    } else {
      throw NotImplemented(pos);
    }
  }

}
