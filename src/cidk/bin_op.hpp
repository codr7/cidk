#ifndef CIDK_BIN_OP_HPP
#define CIDK_BIN_OP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct BinOp: OpType {
    bool commutative;
    
    BinOp(const string &id, bool commutative = false);
    void init(Op &op, const Val &x, const Val &y, Fun *f) const;
    virtual void eval(const Op &op, Env &env, Stack &stack) const override;
    virtual const Sym *get_fun_id(Cx &cx) const = 0;
    virtual void get_ids(const Op &op, IdSet &out) const override;
    virtual void mark_refs(Op &op) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      ReadState &state,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };
}

#endif
