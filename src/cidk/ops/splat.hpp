#ifndef CIDK_SPLAT_HPP
#define CIDK_SPLAT_HPP

#include "cidk/op.hpp"

namespace cidk::ops {
  struct SplatType: OpType {
    SplatType(const string &id);
    void init(Cx &cx, Op &op, const Val &val, const Val &max) const;

    virtual void compile(Cx &cx,
                         Op &op,
                         Env &env,
                         Ops &out,
                         Opts &opts) const override;

    virtual bool eval(Cx &cx, Op &op, Env &env, Reg *regs) const override {
      auto &p(op.pos);
      auto &args(op.args);
      if (!args[0].eval(p, env, regs)) { return false; }
      cx.pop(p).splat(p, args[1].as_int);
      return true;
    }

    virtual void read(Cx &cx, Pos &pos, istream &in, Ops &out) const override;
  };

  extern const SplatType Splat;
}

#endif
