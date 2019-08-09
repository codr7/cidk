#ifndef CIDK_OP_IF_HPP
#define CIDK_OP_IF_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct IfType: OpType {
    IfType(const string &id);
    void init(Cx &cx, Op &op, const Val &cond, const Val &x, const Val &y) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual void eval(Cx &cx,
                      Op &op,
                      Env &env,
                      Reg *regs) const override;
    
    virtual void mark_refs(Op &op) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const IfType If;
}

#endif
