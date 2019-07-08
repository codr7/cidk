#ifndef CIDK_OP_LET_HPP
#define CIDK_OP_LET_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct LetType: OpType {
    LetType(const string &id);
    void init(Op &op, const Sym *key, const Val &val) const;
    virtual void eval(const Op &op, Env &env, Stack &stack) const override;
    void get_ids(const Op &op, IdSet &out) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const LetType Let;
}

#endif
