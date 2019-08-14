#ifndef CIDK_OP_DEFUN_HPP
#define CIDK_OP_DEFUN_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct DefunType: OpType {
    DefunType(const string &id);
    void init(Cx &cx,
              Op &op,
              const Val &id,
              const Val &args,
              const Val &rets,
              const Val &body) const;
    
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

  extern const DefunType Defun;
}

#endif
