#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/val_type.hpp"

namespace cidk {
  static void clone_imp(Call &call) {
    auto &s(call.cx.stack);
    s.emplace_back(s.back());
  }

  static void eq_imp(Call &call) {
    auto &cx(call.cx);
    auto &s(cx.stack);
    auto y(s.back());
    s.pop_back();
    auto &x(s.back());
    x.reset(call.pos, cx.Bool, x.is(call.pos, y));
  }

  ValType::ValType(Cx &cx, const Pos &pos, const string &id): Type(cx, pos, id) { }

  void ValType::init() {
    env.add_fun(pos, "clone", {Arg("it")}, {Ret(cx.Any)}, clone_imp);
    env.add_fun(pos, "eq", {Arg("x"), Arg("y")}, {Ret(cx.Bool)}, eq_imp);
  }

  void ValType::call(const Pos &pos, const Val &val) const {
    cx.stack.emplace_back(val);
  }

  void ValType::compile(const Pos &pos, const Val &val, Ops &out) const {
    cx.stack.push_back(val);
    out.emplace_back(ops::Push, val);
  }
  
  bool ValType::eq(const Pos &pos, const Val &x, const Val &y) const {
    return is(pos, x, y);
  }

  void ValType::move(const Pos &pos, Val &dst, const Val &src) const {
    dup(pos, dst, src);
  }

  void ValType::splat(const Pos &pos, Val &val) {
    cx.stack.push_back(val);
  }

  void ValType::sweep(const Pos &pos, Val &val) { }
}
