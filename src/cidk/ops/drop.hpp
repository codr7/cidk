#ifndef CIDK_OP_DROP_HPP
#define CIDK_OP_DROP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct DropType: OpType {
    DropType(const string &id);
    void init(Cx &cx, Op &op, Int n) const;

    virtual void eval(Cx &cx,
                      Op &op,
                      Env &env,
                      Regs &regs,
                      Stack &stack) const override;
    
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const DropType Drop;
}

#endif
