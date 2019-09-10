#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/cp.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::ops {
  const CpType Cp("cp");

  CpType::CpType(const string &id): OpType(id) {}

  void CpType::init(Cx &cx, Op &op, const Val &offs, const Val &len) const {
    op.args[0] = offs;
    op.args[1] = len;
  }

  void CpType::compile(Cx &cx, Op &op, Env &env, Ops &out, Opts &opts) const {
    auto &p(op.pos);
    auto &args(op.args);
    for (int i(0); i < 2; i++) { args[i].compile(op.pos, env, opts); }
    auto &offs(args[0]);

    if (offs.type != &cx.int_type) {
      throw ESys(p, "Invalid offset: ", offs.type->id);
    }

    auto &len(args[1]);
    
    if (len.type != &cx.int_type) {
      throw ESys(p, "Invalid length: ", len.type->id);
    }
    
    out.push_back(op);
  }

  bool CpType::eval(Cx &cx, Op &op, Env &env, Reg *regs) const {
    auto &p(op.pos);
    auto &args(op.args);
    auto offs(args[0].as_int), len(args[1].as_int);

    if (offs == -1) { offs = cx.stackp - cx.stack.begin() - 1; }
    if (len == -1) { len = offs + 1; }

    if (len == 1) {
      cx.push(p, cx.peek(p, offs));
    } else {
      auto i(cx.stackp - offs - 1);
      copy(i, i + len, cx.stackp);
      cx.stackp += len;
    }

    return true;
  }

  void CpType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Val offs(cx.int_type, Int(0)), len(cx.int_type, Int(1));
    bool done(false);
    Pos p(pos), vp(p);
    
    auto v(read_val(cx, pos, in));
    if (!v) { throw ESys(vp, "Missing ;"); }

    if (v->is_eop()) {
      done = true;
    } else {
      offs = *v;
      
      if (!(v = read_val(cx, pos, in))) { throw ESys(vp, "Missing ;"); }

      if (v->is_eop()) {
        done = true;
      } else {
        len = *v;
      }
    }

    if (!done) { read_eop(pos, in); }
    out.emplace_back(cx, p, *this, offs, len);
  }
}
