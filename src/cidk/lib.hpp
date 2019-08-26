#ifndef CIDK_LIB_HPP
#define CIDK_LIB_HPP

#include "cidk/cx.hpp"
#include "cidk/def.hpp"
#include "cidk/env.hpp"

namespace cidk {
  struct Lib: Def {
    Env env;

    Lib(Cx &cx, const Pos &pos, const Sym *id);

    void mark();
    virtual void sweep(Cx &cx, const Pos &pos) override;
  };

  template <typename LibT, typename...Args>
  LibT &Env::add_lib(Cx &cx, const Pos &pos, const string &id, Args...args) {
    auto &lt(cx.lib_type);
    LibT *l(new LibT(cx, pos, cx.intern(pos, id), forward<Args>(args)...));
    let(cx, pos, l->id, Val(lt, l));
  }

}

#endif
