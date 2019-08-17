#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/dispatch.hpp"
#include "cidk/read.hpp"
#include "cidk/types/list.hpp"

namespace cidk::ops {
  const DispatchType Dispatch("dispatch");

  DispatchType::DispatchType(const string &id): OpType(id) {}

  void DispatchType::init(Cx &cx, Op &op, const Val &funs) const {
    op.args[0] = funs;
  }

  void DispatchType::compile(Cx &cx,
                             OpIter &in,
                             const OpIter &end,
                             Env &env,
                             Ops &out,
                             Opts &opts) const {
    auto &p(in->pos);
    auto &funs(in->args[0]);
    funs.compile(p, env, opts);
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
    auto stack_len(cx.stackp - cx.stack.begin());
    Val *stack_end(&cx.peek(p));
    Fun *ok(nullptr);
    
    for (auto &fv: op.args[0].as_list->items) {
      auto &f(*fv.as_fun);
      auto &args(f.args.items);
      ok = &f;
      
      if (auto nargs(args.size()); nargs) {
        if (nargs > stack_len) { continue; }
        Val *s(stack_end);
        
        for (Arg *a = &args.back(); a >= &args[0]; a--, s--) {          
          if (Type *at(a->type); at != &cx.a_type && !s->type->isa(*at)) {
            ok = nullptr;
            break;
          }
        }
      } else {
        break;
      }
    }

    if (!ok) { throw ESys(p, "Dispatch failed"); } 
    ok->call(cx, p, env);
  }

  void DispatchType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
      
    for (;;) {
      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(p, "Missing ;"); }
      if (v->is_eop()) { break; }
      out.emplace_back(cx, p, *this, *v);
    }
  }
}
