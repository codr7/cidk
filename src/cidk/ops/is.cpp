#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/is.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::ops {
  struct Data {
    Val x, y;
    Data(const Val &x, const Val &y): x(x), y(y) { }
  };
  
  const IsType Is("is");

  IsType::IsType(const string &id): OpType(id) { }

  void IsType::init(Op &op, const Val &x, const Val &y) const {
    op.data = Data(x, y);
  }

  void IsType::eval(const Op &op, Env &env, Stack &stack) const {
    const Pos &p(op.pos);
    const Data &d(op.as<Data>());
    d.x.eval(p, env, stack);
    auto x(pop(p, stack));
    d.y.eval(p, env, stack);
    auto y(pop(p, stack));
    stack.emplace_back(op.pos, env.cx.bool_type, x.is(y));
  }

  void IsType::get_ids(const Op &op, IdSet &out) const {
    const Data &d(op.as<Data>());
    d.x.get_ids(out);
    d.y.get_ids(out);
  }

  void IsType::read(Cx &cx,
                    Pos &pos,
                    istream &in,
                    Env &env,
                    Stack &stack,
                    Ops &out) const {
    Pos p(pos);
    auto x(read_val(pos, in, env, stack));
    if (!x) { throw ESys(p, "Missing x"); }

    auto y(read_val(pos, in, env, stack));
    if (!y) { throw ESys(p, "Missing y"); }
    read_eop(pos, in, env, stack);
    out.emplace_back(p, *this, *x, *y);
  }
}
