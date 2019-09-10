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

  bool PairType::find_op(const Val &val,
                         function<bool (Ops &, OpIter &)> pred) const {
    auto &p(*val.as_pair);
    return p.first.find_op(pred) || p.second.find_op(pred);
  }

  void PairType::mark_refs(const Val &val) const { val.as_pair->mark_refs(); }

  void PairType::sweep(const Pos &pos, Val &val) { val.as_pair->sweep(cx, pos); }
}
