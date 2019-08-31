#ifndef CIDK_OP_ISA_HPP
#define CIDK_OP_ISA_HPP

#include "cidk/op.hpp"

namespace cidk::ops {
  struct IsaType: OpType {
    IsaType(const string &id);
    void init(Cx &cx, Op &op, const Val &parent, const Val &child) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual void eval(Cx &cx, Op &op, Env &env, Reg *regs) const override;

    virtual bool find_op(Op &op,
                         function<bool (Ops &, OpIter &)> pred) const override;

    virtual void mark_refs(Op &op) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const IsaType Isa;
}

#endif
