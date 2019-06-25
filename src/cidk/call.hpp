#ifndef CIDK_CALL_HPP
#define CIDK_CALL_HPP

#include "cidk/pos.hpp"

namespace cidk {  
  struct Cx;
  struct Fun;

  struct Call {
    Cx &cx;
    Pos pos;
    Call *prev;
    Fun &target;

    Call(Cx &cx, const Pos &pos, Fun &target);
    ~Call() noexcept(false);

    void eval();
  };

  
}

#endif
