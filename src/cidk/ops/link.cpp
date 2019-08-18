#include <dlfcn.h>

#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/link.hpp"
#include "cidk/read.hpp"
#include "cidk/types/str.hpp"

namespace cidk::ops {
  const LinkType Link("link");

  LinkType::LinkType(const string &id): OpType(id) {}

  void LinkType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto fname(read_val(cx, pos, in));
    if (!fname || fname->is_eop()) { throw ESys(p, "Missing filename"); }
    if (fname->type != &cx.str_type) { throw ESys(p, "Invalid filename"); } 
    read_eop(pos, in);
    
    auto fn(fname->as_str->to_utf8(cx));
    void *h(dlopen(fn.c_str(), RTLD_LAZY | RTLD_NODELETE));
    if (!h) { throw ESys(p, "Failed linking ", fn, ": ", dlerror()); }
    Pluginit pluginit;
    *reinterpret_cast<void **>(&pluginit) = dlsym(h, "cidk_pluginit");
    pluginit(cx);
    dlclose(h);
  }
}
