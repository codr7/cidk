#ifndef CIDK_OP_STASH_HPP
#define CIDK_OP_STASH_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct StashType: OpType {
    StashType(const string &id);
    void init(Cx &cx, Op &op) const;
    virtual void eval(Op &op, cidk::Env &env, Stack &stack) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      ReadState &state,
                      cidk::Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const StashType Stash;
}

#endif
