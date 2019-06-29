#include "cidk/call.hpp"
#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/val_type.hpp"

namespace cidk {
  static void clone_imp(Call &call) {
    auto &s(call.cx.stack);
    s.emplace_back(s.back());
  }
  
  static void dump_imp(Call &call) {
    auto &cx(call.cx);
    auto &s(cx.stack);
    auto out(s.back());
    s.pop_back();
    auto val(s.back());
    s.pop_back();
    val.type->dump(call.pos, val, out.as_ostream->imp);
  }
  
  static void eq_imp(Call &call) {
    auto &cx(call.cx);
    auto &s(cx.stack);
    auto y(s.back());
    s.pop_back();
    auto &x(s.back());
    x.reset(call.pos, cx.bool_type, x.is(call.pos, y));
  }

  ValType::ValType(Cx &cx, const Pos &pos, const string &id): Type(cx, pos, id) { }

  void ValType::init() {
    env.add_fun(pos, "clone", {Arg("it")}, {Ret(cx.any_type)}, clone_imp);
    env.add_fun(pos, "dump", {Arg("val"), Arg("out")}, {}, dump_imp);
    env.add_fun(pos, "eq", {Arg("x"), Arg("y")}, {Ret(cx.bool_type)}, eq_imp);
  }

  void ValType::add(const Pos &pos, const Val &x, const Val &y) const {
    throw NotImplemented(pos);
  }

  void ValType::call(const Pos &pos, const Val &val) const {
    cx.stack.emplace_back(val);
  }

  void ValType::mark_refs(const Pos &pos, const Val &val) { }

  void ValType::splat(const Pos &pos, Val &val) {
    cx.stack.push_back(val);
  }

  void ValType::sweep(const Pos &pos, Val &val) { }
}
