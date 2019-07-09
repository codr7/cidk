#ifndef CIDK_OP_CALL_HPP
#define CIDK_OP_CALL_HPP

#include "cidk/op.hpp"

namespace cidk::ops {
  struct CallType: OpType {
    CallType(const string &id);
    void init(Op &op, const Val &target) const;
    virtual void eval(const Op &op, Env &env, Stack &stack) const override;
    virtual void get_ids(const Op &op, IdSet &out) const override;

    virtual void read(Cx &cx,
                      Pos &pos,
                      istream &in,
                      ReadState &state,
                      Env &env,
                      Stack &stack,
                      Ops &out) const override;
  };

  extern const CallType Call;
}

#endif
