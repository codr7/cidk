#ifndef CIDK_OP_PUSH_HPP
#define CIDK_OP_PUSH_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct PushType: OpType {
    PushType(const string &id);
    void init(Cx &cx, Op &op, const Val &val) const;

    virtual void compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual bool eval(Cx &cx, Op &op, Env &env, Reg *regs) const override {
      return op.args[0].eval(op.pos, env, regs);
    }

    virtual bool find_op(Op &op,
                         function<bool (Ops &, OpIter &)> pred) const override;

    virtual void mark_refs(Op &op) const override;
    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const PushType Push;
}

#endif
