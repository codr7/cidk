#ifndef CIDK_OP_DROP_HPP
#define CIDK_OP_DROP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct DropType: OpType {
    DropType(const string &id);
    void init(Cx &cx, Op &op, const Val &n) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual bool eval(Cx &cx,
                      Op &op,
                      Env &env,
                      Reg *regs) const override;
    
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const DropType Drop;
}

#endif
