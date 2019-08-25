#include "cidk/cx.hpp"
#include "cidk/types/reg.hpp"
#include "cidk/types/sym.hpp"
#include "cidk/val.hpp"

namespace cidk {
  SymType::SymType(Cx &cx,
                   const Pos &pos,
                   const Sym *id,
                   const vector<Type *> &parents):
    TValType<const Sym *>(cx, pos, id, parents) {}

  void SymType::compile(const Pos &pos, Val &val, Env &env, Opts &opts) const {
    auto s(val.as_sym);
    
    if (auto reg(opts.regs.find(s)); reg == opts.regs.end()) {
      if (auto i(env.try_get(pos, s)); i) {
        i->clone(pos, val);
      } else {
        val.reset(cx.reg_type, opts.push_ext_id(pos, s));
      }
    } else {
      val.reset(cx.reg_type, reg->second);
    }

    val.id = s;
  }

  void SymType::dump(const Val &val, ostream &out) const { out << val.as_sym; }

  void SymType::eval(const Pos &pos, const Val &val, Env &env, Reg *regs) const {
    env.get(pos, val.as_sym).eval(pos, env, regs);
  }

  bool SymType::is(const Val &x, const Val &y) const { return x.as_sym == y.as_sym; }

  void SymType::set(Val &dst, const Sym *val) const { dst.as_sym = val; }
}
