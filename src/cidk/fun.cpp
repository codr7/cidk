#include "cidk/e.hpp"
#include "cidk/fun.hpp"

namespace cidk {
  Fun::Fun(Cx &cx,
           const Pos &pos,
           const string &id,
           initializer_list<Arg> args,
           initializer_list<Ret> rets,
           Imp imp):
    Def(cx, pos, id), env(cx), imp(imp) { }
}
