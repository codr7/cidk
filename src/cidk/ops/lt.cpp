#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/lt.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  struct LtData {
    Val x, y;
    Fun *f;
    
    LtData(const Val &x, const Val &y, Fun *f): x(x), y(y), f(f) {}
  };
  
  const LtType Lt("lt");

  LtType::LtType(const string &id): OpType(id) {}

  void LtType::init(Op &op, const Val &x, const Val &y, Fun *f) const {
    op.data = LtData(x, y, f);
  }

  void LtType::eval(const Op &op, Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    const Pos &p(op.pos);
    auto &d(op.as<LtData>());
    d.y.eval(p, env, stack);
    Val y(pop(p, stack));
    d.x.eval(p, env, stack);
    Val &x(stack.back());
    Fun *f(d.f);
    
    if (!f) {
      ValType *xt(x.type), *yt(y.type);
      auto id((xt == yt) ? cx.intern("<") : cx.intern(str("<[", yt->id, ']')));
      auto &fv(xt->env.get(p, id));
      
      if (fv.type != &cx.fun_type) {
        throw ESys(p, "Expected Fun, was: ", fv.type->id);
      }

      f = fv.as_fun;
    }
    
    stack.push_back(y);
    Call(p, *f).eval(env, stack);
  }

  void LtType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<LtData>());
    d.x.get_ids(out);
    d.y.get_ids(out);
  }

  void LtType::mark_refs(Op &op) const {
    auto &d(op.as<LtData>());
    d.x.mark_refs();
    d.y.mark_refs();
  }

  void LtType::read(Cx &cx,
                    Pos &pos,
                    istream &in,
                    ReadState &state,
                    Env &env,
                    Stack &stack,
                    Ops &out) const {
    Pos p(pos);
    auto x(read_val(pos, in, state, env, stack));
    if (!x) { throw ESys(p, "Missing ;"); }
    Fun *f(cx.int_type.env.get(pos, cx.intern("<")).as_fun);
    
    if (x->is_eop()) { out.emplace_back(p, *this, cx.S, cx.S, f); }
    else {
      auto y(read_val(pos, in, state, env, stack));
      
      if (y->is_eop()) { out.emplace_back(p, *this, *x, cx.S, f); }
      else {
        out.emplace_back(p, *this, *x, *y, f);
        read_eop(pos, in, env, stack);
      }
    }
  }
}
