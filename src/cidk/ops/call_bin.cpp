#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/call_bin.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  const CallBinType CallBin("call-bin");

  CallBinType::CallBinType(const string &id): OpType(id) {}

  void CallBinType::init(Cx &cx, Op &op, const Val &id) const { op.args[0] = id; }

  void CallBinType::compile(Cx &cx,
                            OpIter &in,
                            const OpIter &end,
                            Env &env,
                            Ops &out,
                            Opts &opts) const {
    out.push_back(*in);
  }
  
  void CallBinType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &idv(op.args[0]);
    const Sym *id(nullptr);

    if (idv.type == &cx.sym_type) {
      id = idv.as_sym;
    } else {
      idv.eval(p, env, regs);
      Val &idv(cx.pop(p));
      if (idv.type != &cx.sym_type) { throw ESys(p, "Expected id: ", idv); }
      id = idv.as_sym;
    }

    auto &x(cx.peek(p, 1)), &y(cx.peek(p));
    auto &f(env.get(p, cx.intern(p, str(id, '[', x.type->id, ' ', y.type->id, ']'))));
    if (f.type != &cx.fun_type) { throw ESys(p, "Expected Fun: ", f); }
    f.as_fun->call(cx, p, env);
  }

  void CallBinType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    int n(0);
    
    for (;; n++) {
      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      out.emplace_back(cx, p, *this, *v);
    }

    if (!n) { out.emplace_back(cx, p, *this, cx.$); }
  }
}
