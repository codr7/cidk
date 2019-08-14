#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/list.hpp"
#include "cidk/ops/assert.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/list.hpp"
#include "cidk/types/nil.hpp"

namespace cidk::ops {
  const AssertType Assert("assert");

  AssertType::AssertType(const string &id): OpType(id) {}

  void AssertType::init(Cx &cx, Op &op, const Val &msg, const Val &body) const {
    op.args[0] = msg;
    op.args[1] = body;
  }

  void AssertType::compile(Cx &cx,
                           OpIter &in,
                           const OpIter &end,
                           Env &env,
                           Ops &out,
                           Opts &opts) const {
    auto &p(in->pos);
    auto &args(in->args);
    for (int i(0); i < 2; i++) { args[i].compile(p, env, opts); }

    auto &msg(args[0]);
    if (msg.type != &cx.list_type) { throw ESys(p, "Expected List: ", msg.type->id); }
    out.push_back(*in);
  }

  void AssertType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);
    
    args[0].eval(p, env, regs);
    Val msg(cx.pop(p));

    args[1].eval(p, env, regs);
    auto &ok(cx.pop(p));
    
    if (ok.type != &cx.bool_type) {
      throw ESys(p, "Expected Bool: ", ok.type->id);
    }

    if (!ok.as_bool) { throw ESys(p, "Test failed: ", msg); }
  }

  void AssertType::mark_refs(Op &op) const {
    auto &args(op.args);
    for (int i(0); i < 2; i++) { args[i].mark_refs(); }
  }

  void AssertType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto msg(read_val(cx, pos, in));
    if (!msg || msg->is_eop()) { throw ESys(p, "Missing message"); }
    auto body(read_val(cx, pos, in));
    if (!body || body->is_eop()) { throw ESys(p, "Missing body"); }
    read_eop(pos, in);
    out.emplace_back(cx, pos, *this, *msg, *body);
  }
}
