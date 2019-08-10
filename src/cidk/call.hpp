#ifndef CIDK_CALL_HPP
#define CIDK_CALL_HPP

#include "cidk/pos.hpp"

namespace cidk {  
  struct Cx;
  struct Env;
  struct Fun;
  struct Sym;
  
  struct Call {
    const Pos &pos;
    Call *prev;
    Fun &fun;

    Call(const Pos &pos, Fun &fun);
    ~Call() noexcept(false);

    void eval(Cx &cx);
  };
}

#endif
