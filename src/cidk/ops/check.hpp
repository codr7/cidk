#ifndef CIDK_OP_CHECK_HPP
#define CIDK_OP_CHECK_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct CheckType: OpType {
    CheckType(const string &id);
    void init(Cx &cx, Op &op, const Val &msg, const Val &body) const;

    virtual void compile(Cx &cx,
                         Op &op,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual bool eval(Cx &cx, Op &op, Env &env, Reg *regs) const override;
    
    virtual bool find_op(Op &op,
                         function<bool (Ops &, OpIter &)> pred) const override;

    virtual void mark_refs(Op &op) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const CheckType Check;
}

#endif
