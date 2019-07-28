#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/list.hpp"
#include "cidk/ops/assert.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"
#include "cidk/types/list.hpp"
#include "cidk/types/nil.hpp"

namespace cidk::ops {
  struct AssertData {
    Val args, body;
    AssertData(const Val &args, const Val &body): args(args), body(body) {}
  };

  const AssertType Assert("assert");

  AssertType::AssertType(const string &id): OpType(id) {}

  void AssertType::init(Cx &cx, Op &op, const Val &args, const Val &body) const {
    op.data = AssertData(args, body);
  }

  void AssertType::compile(Cx &cx,
                           OpIter &in,
                           const OpIter &end,
                           Env &env,
                           Stack &stack,
                           Ops &out,
                           Opts *opts) const {
    auto &d(in->as<AssertData>());
    d.args.compile(cx, in->pos, env, stack, opts);
    d.body.compile(cx, in->pos, env, stack, opts);
    out.push_back(*in);
  }

  void AssertType::eval(Cx &cx, Op &op, Env &env, Stack &stack) const {
    const Pos &p(op.pos);
    const auto &d(op.as<AssertData>());
    Stack args;
    
    if (!d.args.as_list->items.empty()) {
      copy(stack.begin(), stack.end(), back_inserter(args));
      for (auto &a: d.args.as_list->items) { a.push(cx, p, env, args); }
    }
    
    d.body.eval(cx, p, env, stack);
    auto &ok(pop(op.pos, stack));
    
    if (ok.type != &cx.bool_type) {
      throw ESys(p, "Expected Bool, was: ", ok.type->id);
    }

    if (!ok.as_bool) { throw ESys(p, "Test failed: ", args); }
  }

  void AssertType::get_ids(const Op &op, IdSet &out) const {
    auto &d(op.as<AssertData>());
    d.args.get_ids(out);
    d.body.get_ids(out);
  }

  void AssertType::mark_refs(Op &op) const {
    auto &d(op.as<AssertData>());
    d.args.mark_refs();
    d.body.mark_refs();
  }

  void AssertType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Pos p(pos);
    auto args(read_val(cx, pos, in));

    if (args->type != &cx.list_type) {
      throw ESys(p, "Expected List, was: ", args->type->id);
    }

    auto body(read_val(cx, pos, in));
    read_eop(pos, in);
    out.emplace_back(cx, pos, *this, *args, *body);
  }
}
