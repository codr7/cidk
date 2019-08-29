#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/call.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::ops {
  const CallType Call("call");

  CallType::CallType(const string &id): OpType(id) {}

  void CallType::init(Cx &cx, Op &op, const Val &fun, const Val &check) const {
    op.args[0] = fun;
    op.args[1] = check;
  }

  void CallType::compile(Cx &cx,
                         OpIter &in,
                         const OpIter &end,
                         Env &env,
                         Ops &out,
                         Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);
    for (int i(0); i < 2; i++) { args[i].compile(p, env, opts); }
    auto &check(args[1]);
    if (check.type != &cx.bool_type) { throw ESys(p, "Expected Bool: ", check); }
    out.push_back(*in);
  }

  void CallType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);
    auto &fv(args[0]);
    Fun *f(nullptr);
    
    if (fv.type == &cx.fun_type) {
      f = fv.as_fun;
    } else {
      fv.eval(p, env, regs);
      auto &fv(cx.pop(p));
      if (fv.type != &cx.fun_type) { throw ESys(p, "Invalid call target: ", fv); }
      f = fv.as_fun;
    }
    
    if (cx.debug || args[1].as_bool) {
      auto stack_len(cx.stackp - cx.stack.begin());

      if (!f->match(stack_len ? &cx.peek(p) : cx.stackp, 0, stack_len)) {
        throw ESys(p, "Call failed: ", f->id);
      }
    }
      
    f->call(cx, p, env);
  }

  void CallType::mark_refs(Op &op) const { op.args[0].mark_refs(); }

  void CallType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto fun(read_val(cx, pos, in));
    if (!fun) { throw ESys(p, "Missing ;"); }
    Val check(cx.bool_type, false);
    
    if (fun->is_eop()) {
      fun = cx.$;
    } else {
      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing ;"); }

      if (!v->is_eop()) {
        check = *v;
        read_eop(p, in);
      }
    }
    
    out.emplace_back(cx, p, *this, *fun, check);
  }
}
