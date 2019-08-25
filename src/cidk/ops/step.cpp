#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/step.hpp"
#include "cidk/read.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/types/reg.hpp"
#include "cidk/types/sym.hpp"

namespace cidk::ops {
  const StepType Step("step");

  StepType::StepType(const string &id): OpType(id) {}

  void StepType::init(Cx &cx, Op &op,
                      const Val &place,
                      const Val &delta,
                      const Val &push) const {
    op.args[0] = place;
    op.args[1] = delta;
    op.args[2] = push;
  }

  void StepType::compile(Cx &cx,
                        OpIter &in,
                        const OpIter &end,
                        Env &env,
                        Ops &out,
                        Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);
    for (int i(0); i < 3; i++) { args[i].compile(p, env, opts); }
    auto &delta(args[1]);

    if (delta.type != &cx.int_type) {
      auto tid(args[1].type->id);
      args[3] = env.get(p, cx.intern(p, str("+[", tid, ' ', tid, ']')));
    }
    
    out.push_back(*in);
  }

  void StepType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);
    auto &place(args[0]), &delta(args[1]);
    bool push(args[2].as_bool);
    
    if (place.type == &cx.reg_type) {
      Val &v(regs[place.as_reg]);
      
      if (delta.type == &cx.int_type) {
        v.as_int += delta.as_int;
        if (push) { cx.push(p, cx.int_type, v.as_int); }
      } else {
        cx.push(p, v);
        cx.push(p, delta);
        args[3].as_fun->call(cx, p, env);
        v = push ? cx.peek(p) : cx.pop(p);
        v.id = place.id;
      }

    } else {
      place.eval(p, env, regs);

      if (delta.type == &cx.int_type) {
        cx.peek(p).as_int += delta.as_int;
      } else {
        cx.push(p, delta);      
        args[3].as_fun->call(cx, p, env);
      }
    }
  }

  void StepType::mark_refs(Op &op) const {
    auto &args(op.args);
    for (int i(0); i < 3; i++) { args[i].mark_refs(); }

    auto &fun(args[3]);
    if (fun.type) { fun.mark_refs(); }
  }

  void StepType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    Val place(cx.$), delta(cx.int_type, Int(1)), push(cx.T);

    auto v(read_val(cx, pos, in));
    if (!v) { throw ESys(p, "Missing ;"); }
    
    if (!v->is_eop()) {
      place = *v;
      v = read_val(cx, pos, in);
      if (!v) { throw ESys(p, "Missing ;"); }

      if (!v->is_eop()) {
        delta = *v;
        v = read_val(cx, pos, in);
        if (!v) { throw ESys(p, "Missing ;"); }

        if (!v->is_eop()) {
          push = *v;
          read_eop(pos, in);
        }
      }
    }
    
    out.emplace_back(cx, p, *this, place, delta, push);
  }
}
