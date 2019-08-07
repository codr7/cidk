#ifndef CIDK_OP_DEC_HPP
#define CIDK_OP_DEC_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct DecType: OpType {
    DecType(const string &id);
    void init(Cx &cx, Op &op, const Val &n, const Val &delta) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Stack &stack,
                         Ops &out,
                         Opts &opts) const override;

    virtual void eval(Cx &cx,
                      Op &op,
                      Env &env,
                      Regs &regs,
                      Stack &stack) const override;
    
    virtual void get_ids(const Op &op, IdSet &out) const override;
    virtual void mark_refs(Op &op) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const DecType Dec;
}

#endif
