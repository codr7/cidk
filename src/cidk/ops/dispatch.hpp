#ifndef CIDK_DISPATCH_HPP
#define CIDK_DISPATCH_HPP

#include "cidk/op.hpp"

namespace cidk::ops {
  struct DispatchType: OpType {
    DispatchType(const string &id);
    void init(Cx &cx, Op &op, const Val &funs, const Val &nargs) const;

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
    
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const DispatchType Dispatch;
}

#endif
