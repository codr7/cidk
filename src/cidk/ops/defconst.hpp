#ifndef CIDK_OP_DEFINE_HPP
#define CIDK_OP_DEFINE_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct DefconstType: OpType {
    DefconstType(const string &id);
    void init(Cx &cx, Op &op, const Sym *id, const Val &val) const;

    virtual void compile(Cx &cx,
                         Op &op,
                         Env &env,
                         Stack &stack,
                         Ops &out,
                         Opts *opts) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const DefconstType Defconst;
}

#endif
