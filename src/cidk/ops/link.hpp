#ifndef CIDK_OP_LINK_HPP
#define CIDK_OP_LINK_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct LinkType: OpType {
    LinkType(const string &id);

    void init(Cx &cx, Op &op, const Val &path) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  using Pluginit = void (*)(Cx &);

  extern const LinkType Link;
}

#endif
