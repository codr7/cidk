#ifndef CIDK_BIN_OP_HPP
#define CIDK_BIN_OP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct BinOp: OpType {
    bool is_vararg;
    
    BinOp(const string &id, bool is_vararg);
    void init(Cx &cx, Op &op, const Val &x, const Val &y, Fun *f) const;

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
    
    virtual const Sym *get_fun_id(Cx &cx) const = 0;
    virtual void get_ids(const Op &op, IdSet &out) const override;
    virtual void mark_refs(Op &op) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };
}

#endif
