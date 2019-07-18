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
  
  BinOp::BinOp(const string &id, bool is_assoc):
    OpType(id), is_assoc(is_assoc) {}

  void BinOp::init(Cx &cx, Op &op, const Val &x, const Val &y, Fun *fun) const {
    op.data = BinOpData(x, y, fun);
  }

  void BinOp::eval(Op &op, Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    const Pos &p(op.pos);
    auto &d(op.as<BinOpData>());
    d.x.eval(p, env, stack);
    Val &x(stack.back());
    d.y.eval(p, env, stack);
    Val &y(stack.back());
    Fun *f(d.fun);
    
    if (!f) {
      ValType *xt(x.type), *yt(y.type);
      const Sym *fun_id(get_fun_id(cx));
      auto id((xt == yt) ? fun_id : cx.intern(str(fun_id->name, '[', yt->id, ']')));
      auto &fv(xt->env.get(p, id));
      
      if (fv.type != &cx.fun_type) {
        throw ESys(p, "Expected Fun, was: ", fv.type->id);
      }

      f = fv.as_fun;
    }
    
    Call(p, *f).eval(env, stack);
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

  void BinOp::read(Cx &cx,
                   Pos &pos,
                   istream &in,
                   ReadState &state,
                   Env &env,
                   Stack &stack,
                   Ops &out) const {
    Pos p(pos);
    auto x(read_val(pos, in, state, env, stack));
    if (!x) { throw ESys(p, "Missing ;"); }
    const Sym *fun_id(get_fun_id(cx));
    Fun *f(cx.int_type.env.get(pos, fun_id).as_fun);
    
    if (x->is_eop()) { out.emplace_back(cx, p, *this, cx.$, cx.$, f); }
    else {
      auto y(read_val(pos, in, state, env, stack));
      
      if (y->is_eop()) { out.emplace_back(cx, p, *this, *x, cx.$, f); }
      else {
        out.emplace_back(cx, p, *this, *x, *y, f);

        if (is_assoc) {
          for (;;) {
            auto z(read_val(pos, in, state, env, stack));
            if (!z) { throw ESys(p, "Missing ;"); }
            if (z->is_eop()) { break; }
            out.emplace_back(cx, p, *this, cx.$, *z, f);
          }
        } else { read_eop(p, in, env, stack); }
      }
    }
  }
}
