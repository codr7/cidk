#include <dlfcn.h>

#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/link.hpp"
#include "cidk/read.hpp"
#include "cidk/types/str.hpp"

namespace cidk::ops {
  const LinkType Link("link");

  LinkType::LinkType(const string &id): OpType(id) {}

  void LinkType::init(Cx &cx, Op &op, const Val &fname) const { op.args[0] = fname; }
  
  void LinkType::compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const {
    auto &p(in->pos);
    auto &fname(in->args[0]);
    fname.compile(p, env, opts);
    if (fname.type != &cx.str_type) { throw ESys(p, "Invalid filename: ", fname); } 
    void *h(dlopen(fname.as_str->to_utf8(cx).c_str(), RTLD_LAZY));
    if (!h) { throw ESys(p, "Failed linking ", fname, ": ", dlerror()); }
    Pluginit pluginit;
    *reinterpret_cast<void **>(&pluginit) = dlsym(h, "cidk_pluginit");
    pluginit(cx);
    dlclose(h);
    out.push_back(*in);
  }

  void LinkType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto v(read_val(cx, pos, in));
    if (!v || v->is_eop()) { throw ESys(p, "Missing filename"); }
    read_eop(pos, in);
    out.emplace_back(cx, p, *this, *v);
  }
}
