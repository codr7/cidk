#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/dispatch.hpp"
#include "cidk/read.hpp"
#include "cidk/types/list.hpp"

namespace cidk::ops {
  const DispatchType Dispatch("dispatch");

  DispatchType::DispatchType(const string &id): OpType(id) {}

  void DispatchType::init(Cx &cx, Op &op, const Val &funs, const Val &nargs) const {
    op.args[0] = funs;
    op.args[1] = nargs;
  }

  void DispatchType::compile(Cx &cx,
                             OpIter &in,
                             const OpIter &end,
                             Env &env,
                             Ops &out,
                             Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);
    for (int i(0); i < 2; i ++) { args[i].compile(p, env, opts); }

    if (Val &nargs(args[1]); nargs.type != &cx.int_type) {
      throw ESys(p, "Invalid number of arguments: ", nargs);
    }
    
    auto &funs(args[0]);
    auto stackp(cx.stackp);
    funs.splat(p, -1);

    List *l(cx.list_type.pool.get(cx, stackp, cx.stackp));
    funs.reset(cx.list_type, l);
    cx.stackp = stackp;

    for (Val &fv: l->items) {
      if (fv.type != &cx.fun_type) { throw ESys(p, "Expected Fun: ", fv); }
    }

    l->sort(p, [](auto &pos, auto &x, auto &y) {
        auto &xf(*x.as_fun), &yf(*y.as_fun);
        int narg_cmp(cmp(pos, yf.args.items.size(), xf.args.items.size()));
        return narg_cmp ? narg_cmp : cmp(pos, yf.weight, xf.weight);
      });

    out.push_back(*in);
  }
  
  void DispatchType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);
    Int nargs(args[1].as_int);
    size_t stack_len(cx.stackp - cx.stack.begin());

    if (nargs == -1) {
      nargs = stack_len;
    } else if (nargs > stack_len) {
      throw ESys(p, "Missing arguments");
    }
    
    Val *stack_end(cx.stackp - 1);
    Fun *ok(nullptr);
    
    for (auto &fv: op.args[0].as_list->items) {
      auto &f(*fv.as_fun);

      if (f.match(stack_end, nargs)) {
        ok = &f;
        break;
      }
    }

    if (!ok) { throw ESys(p, "Dispatch failed"); } 
    ok->call(cx, p, env);
  }

  void DispatchType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);

    auto funs(read_val(cx, pos, in));
    if (!funs || funs->is_eop()) { throw ESys(p, "Missing function"); }

    Val nargs(cx.int_type, Int(-1));
    auto v(read_val(cx, pos, in));

    if (!v->is_eop()) {
      nargs = *v;
      read_eop(pos, in);
    }
    
    out.emplace_back(cx, p, *this, *funs, nargs);
  }
}
