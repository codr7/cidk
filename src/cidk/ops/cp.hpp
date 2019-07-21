#ifndef CIDK_OP_CP_HPP
#define CIDK_OP_CP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct CpType: OpType {
    CpType(const string &id);
    void init(Cx &cx, Op &op, const Val &offs, const Val &len) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Stack &stack,
                         Ops &out,
                         Opts *opts) const override;

    virtual void eval(Op &op, Env &env, Stack &stack) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const CpType Cp;
}

#endif
