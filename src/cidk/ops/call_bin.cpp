#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/call_bin.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const CallBinType CallBin("call-bin");

  CallBinType::CallBinType(const string &id): OpType(id) {}

  void CallBinType::init(Cx &cx, Op &op, const Sym *id) const { op.data = id; }

  void CallBinType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto id(op.as<const Sym *>());

    if (!id) {
      auto &idv(cx.pop(p));
      if (idv.type != &cx.sym_type) { throw ESys(p, "Expected id: ", idv.type->id); }
      id = idv.as_sym;
    }
    
    auto &x(cx.peek(p, 1)), &y(cx.peek(p));
    auto &f(env.get(p, cx.intern(str(id, '[', x.type->id, ' ', y.type->id, ']'))));
    if (f.type != &cx.fun_type) { throw ESys(p, "Expected Fun, was: ", f.type->id); }
    f.as_fun->call(cx, p, env);
  }

  void CallBinType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;; n++) {
      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      if (v->type != &cx.sym_type) { throw ESys(p, "Expected id: ", v->type->id); }
      out.emplace_back(cx, p, *this, v->as_sym);
    }

    if (!n) { out.emplace_back(cx, p, *this, nullptr); }
  }
}
