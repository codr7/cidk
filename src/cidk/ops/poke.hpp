#ifndef CIDK_OP_POKE_HPP
#define CIDK_OP_POKE_HPP

#include "cidk/op.hpp"
#include "cidk/val.hpp"

namespace cidk::ops {
  struct PokeType: OpType {
    PokeType(const string &id);
    void init(Cx &cx, Op &op, size_t offs, const Val &val) const;
    virtual void eval(Op &op, Env &env, Stack &stack) const override;
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

  extern const PokeType Poke;
}

#endif
