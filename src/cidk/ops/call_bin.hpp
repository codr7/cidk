#ifndef CIDK_CALL_BIN_HPP
#define CIDK_CALL_BIN_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct CallBinType: OpType {
    CallBinType(const string &id);
    void init(Cx &cx, Op &op, const Sym *id) const;

    virtual void eval(Cx &cx,
                      Op &op,
                      Env &env,
                      Reg *regs) const override;
    
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const CallBinType CallBin;
}

#endif
