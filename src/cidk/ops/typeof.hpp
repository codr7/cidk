#ifndef CIDK_OP_TYPEOF_HPP
#define CIDK_OP_TYPEOF_HPP

#include "cidk/op.hpp"

namespace cidk::ops {
  struct TypeofType: OpType {
    TypeofType(const string &id);
    void init(Cx &cx, Op &op, const Val &val) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual void eval(Cx &cx, Op &op, Env &env, Reg *regs) const override;
    virtual void mark_refs(Op &op) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const TypeofType Typeof;
}

#endif