#ifndef CIDK_CALL_HPP
#define CIDK_CALL_HPP

#include "cidk/pos.hpp"
#include "cidk/stack.hpp"

namespace cidk {  
  struct Cx;
  struct Env;
  struct Fun;

  struct Call {
    const Pos &pos;
    Call *prev;
    Fun &target;

    Call(const Pos &pos, Fun &target);
    ~Call() noexcept(false);

    void eval(Env &env, Stack &stack);
  };

  
}

#endif
