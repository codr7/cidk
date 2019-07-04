#ifndef CIDK_MACRO_HPP
#define CIDK_MACRO_HPP

#include <functional>

#include "cidk/def.hpp"
#include "cidk/ops.hpp"
#include "cidk/stack.hpp"

namespace cidk {
  struct Env;
  struct Pos;
  
  struct Macro: Def {
    using Imp = function<void (const Macro &,
                               Pos &,
                               istream &,
                               Env &, Stack &,
                               Ops &)>;

    Imp imp;

    Macro(Cx &cx, const Pos &pos, const Sym *id, Imp imp);

    void call(Pos &pos, istream &in, Env &env, Stack &stack, Ops &out) const;
    void mark(const Pos &pos);
    virtual void sweep(const Pos &pos) override;
  };
}

#endif
