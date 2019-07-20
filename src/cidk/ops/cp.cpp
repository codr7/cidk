#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/cp.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::ops {
  struct CpData {
    Val offs, len;
    CpData(const Val &offs, const Val &len): offs(offs), len(len) {}
  };
    
  const CpType Cp("cp");

  CpType::CpType(const string &id): OpType(id) {}

  void CpType::init(Cx &cx, Op &op, const Val &offs, const Val &len) const {
    op.data = CpData(offs, len);
  }

  void CpType::compile(Cx &cx,
                       Op &op,
                       Env &env,
                       Stack &stack,
                       Ops &out,
                       Opts *opts) const {
    auto &d(op.as<CpData>());
    d.offs.compile(cx, op.pos, env, stack, opts);

    if (d.offs.type == &cx.bool_type && d.offs.as_bool) {
      d.offs.reset(op.pos, cx.int_type, Int(-1));
    } else if (d.offs.type != &cx.int_type) {
      throw ESys(op.pos, "Expected Int, was: ", d.offs.type->id);
    }

    d.len.compile(cx, op.pos, env, stack, opts);

    if (d.len.type == &cx.bool_type && d.len.as_bool) {
      d.len.reset(op.pos, cx.int_type, Int(-1));
    } else if (d.len.type != &cx.int_type) {
      throw ESys(op.pos, "Expected Int, was: ", d.len.type->id);
    }

    out.push_back(op);
  }

  void CpType::eval(Op &op, Env &env, Stack &stack) const {
    const auto &d(op.as<CpData>());
    
    auto
      offs((d.offs.as_int == -1) ? stack.size() : d.offs.as_int),
      len((d.len.as_int == -1) ? offs : d.len.as_int);

    auto i(stack.end() - offs), j(i + len);
    copy(i, j, back_inserter(stack));
  }

  void CpType::read(Cx &cx,
                    Pos &pos,
                    istream &in,
                    Env &env,
                    Stack &stack,
                    Ops &out) const {
    optional<Val> offs, len;
    bool done(false);
    Pos p(pos), vp(p);

    if (!(offs = read_val(pos, in, env, stack))) {
      throw ESys(vp, "Missing ;");
    }

    if (offs->is_eop()) {
      offs.emplace(pos, cx.int_type, Int(1));
      len.emplace(pos, cx.int_type, Int(1));
      done = true;
    } else {      
      if (!(len = read_val(pos, in, env, stack))) {
        throw ESys(vp, "Missing ;");
      }

      if (len->is_eop()) {
        len.emplace(pos, cx.int_type, Int(1));
        done = true;
      }
    }

    if (!done) { read_eop(pos, in, env, stack); }
    out.emplace_back(cx, p, *this, *offs, *len);
  }
}
