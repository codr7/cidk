#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/bin_op.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  struct BinOpData {
    Val x, y;
    Fun *fun;
    
    BinOpData(const Val &x, const Val &y, Fun *fun): x(x), y(y), fun(fun) {}
  };
  
  BinOp::BinOp(const string &id, bool is_vararg):
    OpType(id), is_vararg(is_vararg) {}

  void BinOp::init(Cx &cx, Op &op, const Val &x, const Val &y, Fun *fun) const {
    op.data = BinOpData(x, y, fun);
  }

  void BinOp::compile(Cx &cx,
                      OpIter &in,
                      const OpIter &end,
                      Env &env,
                      Stack &stack,
                      Ops &out,
                      Opts *opts) const {
    auto &d(in->as<BinOpData>());
    d.x.compile(cx, in->pos, env, stack, opts);
    d.y.compile(cx, in->pos, env, stack, opts);
    out.push_back(*in);
  }

  void BinOp::eval(Cx &cx, Op &op, Env &env, Stack &stack) const {
    const Pos &p(op.pos);
    auto &d(op.as<BinOpData>());
    Fun *f(d.fun);
    d.x.eval(cx, p, env, stack);
    d.y.eval(cx, p, env, stack);
    
    if (!f) {
      auto i(stack.end()-1), j(i-1);
      ValType *xt(i->type), *yt(j->type);
      const Sym *fun_id(get_fun_id(cx));
      auto id((xt == yt) ? fun_id : cx.intern(str(fun_id->name, '[', yt->id, ']')));
      auto &fv(xt->env.get(p, id));
      
      if (fv.type != &cx.fun_type) {
        throw ESys(p, "Expected Fun, was: ", fv.type->id);
      }

      f = fv.as_fun;
    }
    
    Call(p, *f).eval(cx, env, stack);
  }

  void BinOp::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<BinOpData>());
    d.x.get_ids(out);
    d.y.get_ids(out);
  }

  void BinOp::mark_refs(Op &op) const {
    auto &d(op.as<BinOpData>());
    d.x.mark_refs();
    d.y.mark_refs();
  }

  void BinOp::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto x(read_val(cx, pos, in));
    if (!x) { throw ESys(p, "Missing ;"); }
    const Sym *fun_id(get_fun_id(cx));
    Fun *f(cx.int_type.env.get(pos, fun_id).as_fun);
    
    if (x->is_eop()) {
      out.emplace_back(cx, p, *this, cx.$, cx.$, f);
    } else {
      auto y(read_val(cx, pos, in));
      
      if (y->is_eop()) {
        out.emplace_back(cx, p, *this, *x, cx.$, f);
      } else {
        out.emplace_back(cx, p, *this, *x, *y, f);

        if (is_vararg) {
          for (;;) {
            auto z(read_val(cx, pos, in));
            if (!z) { throw ESys(p, "Missing ;"); }
            if (z->is_eop()) { break; }
            out.emplace_back(cx, p, *this, cx.$, *z, f);
          }
        } else {
          read_eop(p, in);
        }
      }
    }
  }
}
