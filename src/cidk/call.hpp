#ifndef CIDK_CALL_HPP
#define CIDK_CALL_HPP

#include "cidk/pos.hpp"
#include "cidk/stack.hpp"

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

    void eval(Env &env, Stack &stack);
    void eval(Fun &fun, Env &env, Stack &stack);
    void forward(Env &dst, const Sym *id, Env &env, Stack &stack);
  };

  
}

#endif
