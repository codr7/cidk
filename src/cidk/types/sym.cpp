#include <iostream>

#include "cidk/cx.hpp"
#include "cidk/types/env.hpp"
#include "cidk/types/reg.hpp"
#include "cidk/types/sym.hpp"
#include "cidk/val.hpp"

namespace cidk {
  SymType::SymType(Cx &cx,
                   const Pos &pos,
                   const Sym *id,
                   const vector<Type *> &parents):
    TValType<const Sym *>(cx, pos, id, parents) {}

  void SymType::compile(Cx &cx,
                        const Pos &pos,
                        Val &val,
                        Env &env,
                        Stack &stack,
                        Opts &opts) const {
    auto s(val.as_sym);
    
    if (auto reg(opts.regs.find(s)); reg == opts.regs.end()) {
      if (auto i(env.try_get(s)); i) {
        i->val.clone(pos, val);
      } else {
        val.reset(cx.reg_type, opts.push_ext_id(s));
      }
    } else {
      val.reset(cx.reg_type, reg->second);
    }
  }

  void SymType::cp(Val &dst, const Val &src) const { dst.as_sym = src.as_sym; }

  void SymType::dump(const Val &val, ostream &out) const { out << val.as_sym; }

  void SymType::eval(Cx &cx,
                     const Pos &pos,
                     const Val &val,
                     Env &env,
                     Reg *regs,
                     Stack &stack) const {
    env.get(pos, val.as_sym).eval(cx, pos, env, regs, stack);
  }

  bool SymType::is(const Val &x, const Val &y) const { return x.as_sym == y.as_sym; }

  void SymType::set(Val &dst, const Sym *val) const { dst.as_sym = val; }
}
