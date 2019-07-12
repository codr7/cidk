#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/add.hpp"
#include "cidk/read.hpp"

namespace cidk::ops {
  struct AddData {
    Val x, y;
    AddData(const Val &x, const Val &y): x(x), y(y) {}
  };
  
  const AddType Add("add");

  AddType::AddType(const string &id): OpType(id) {}

  void AddType::init(Op &op, const Val &x, const Val &y) const {
    op.data = AddData(x, y);
  }

  void AddType::eval(const Op &op, Env &env, Stack &stack) const {
    Cx &cx(env.cx);
    const Pos &p(op.pos);
    auto &d(op.as<AddData>());
    d.x.eval(p, env, stack);
    d.y.eval(p, env, stack);
    
    auto i(stack.end()-1);
    Val &y(*i--), &x(*i);
    ValType *xt(x.type), *yt(y.type);
    auto id((xt == yt) ? cx.add_id : cx.intern(str("+[", yt->id, ']')));
    xt->env.call(p, id, env, stack);
  }

  void AddType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<AddData>());
    d.x.get_ids(out);
    d.y.get_ids(out);
  }

  void AddType::mark_refs(Op &op) const {
    auto &d(op.as<AddData>());
    d.x.mark_refs();
    d.y.mark_refs();
  }

  void AddType::read(Cx &cx,
                    Pos &pos,
                    istream &in,
                    ReadState &state,
                    Env &env,
                    Stack &stack,
                    Ops &out) const {
    Pos p(pos);
    auto x(read_val(pos, in, state, env, stack));
    if (!x) { throw ESys(p, "Missing ;"); }
    
    if (x->is_eop()) { out.emplace_back(p, *this, cx.S, cx.S); }
    else {
      auto y(read_val(pos, in, state, env, stack));
      if (!y) { throw ESys(p, "Missing ;"); }
      if (!y->is_eop()) { read_eop(pos, in, env, stack); }
      out.emplace_back(p, *this, *x, y->is_eop() ? cx.S : *y);
    }
  }
}
