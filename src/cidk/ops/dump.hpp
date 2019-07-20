#ifndef CIDK_OP_DUMP_HPP
#define CIDK_OP_DUMP_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct DumpType: OpType {
    DumpType(const string &id);
    void init(Cx &cx, Op &op, const Val &val) const;

    virtual void compile(Cx &cx,
                         Op &op,
                         Env &env,
                         Stack &stack,
                         Ops &out,
                         Opts *opts) const override;

    virtual void eval(Op &op, Env &env, Stack &stack) const override;
    virtual void get_ids(const Op &op, IdSet &out) const override;
    virtual void mark_refs(Op &op) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const DumpType Dump;
}

#endif
