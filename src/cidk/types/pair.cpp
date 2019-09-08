#include "cidk/cx.hpp"
#include "cidk/types/pair.hpp"
#include "cidk/val.hpp"

namespace cidk {
  PairType::PairType(Cx &cx,
                     const Pos &pos,
                     Env &env,
                     const Sym *id,
                     const vector<Type *> &parents):
    TValType<Pair *>(cx, pos, env, id, parents) {}

  void PairType::clone(const Pos &pos, Val &dst, const Val &src) const {
    dst.as_pair = cx.pair_type.pool.get(cx, *src.as_pair);
  }

  void PairType::compile(const Pos &pos,
                         Val &val,
                         Env &env,
                         Opts &opts) const {
    auto &p(*val.as_pair);
    p.first.compile(pos, env, opts);
    p.second.compile(pos, env, opts);
  }
  
  void PairType::dump(const Val &val, ostream &out) const {
    auto &p(*val.as_pair);
    out << p.first << ':' << p.second;
  }

  bool PairType::eq(const Pos &pos, const Val &x, const Val &y) const {
    auto &xp(*x.as_pair), &yp(*y.as_pair);
    return xp.first.eq(pos, yp.first) && xp.second.eq(pos, yp.second);
  }

  bool PairType::eval(const Pos &pos,
                      const Val &val,
                      Env &env,
                      Reg *regs) const {
    auto &p(*val.as_pair);
    
    if (!p.first.eval(pos, env, regs) || !p.second.eval(pos, env, regs)) {
      return false;
    }

    Val &right(cx.pop(pos)), &left(cx.pop(pos));

    if (left.is(p.first) && right.is(p.second)) {
      cx.push(pos, val);
    } else {
      cx.push(pos, cx.pair_type, cx.pair_type.pool.get(cx, left, right));
    }
    
    return true;
  }

  bool PairType::find_op(const Val &val,
                         function<bool (Ops &, OpIter &)> pred) const {
    auto &p(*val.as_pair);
    return p.first.find_op(pred) || p.second.find_op(pred);
  }

  bool PairType::is(const Val &x, const Val &y) const {
    auto &xp(*x.as_pair), &yp(*y.as_pair);
    return xp.first.is(yp.first) && xp.second.is(yp.second);
  }

  void PairType::mark_refs(const Val &val) const { val.as_pair->mark_refs(); }

  void PairType::set(Val &dst, Pair *val) const { dst.as_pair = val; }

  void PairType::sweep(const Pos &pos, Val &val) { val.as_pair->sweep(cx, pos); }
}
