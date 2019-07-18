#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/ops/cp.hpp"
#include "cidk/read.hpp"
#include "cidk/types/bool.hpp"

namespace cidk::ops {
  struct CpData {
    Int offs, len;
    CpData(Int offs, Int len): offs(offs), len(len) {}
  };
    
  const CpType Cp("cp");

  CpType::CpType(const string &id): OpType(id) {}

  void CpType::init(Cx &cx, Op &op, Int offs, Int len) const {
    op.data = CpData(offs, len);
  }

  void CpType::eval(Op &op, Env &env, Stack &stack) const {
    const auto &d(op.as<CpData>());
    
    auto
      offs((d.offs == -1) ? stack.size() : d.offs),
      len((d.len == -1) ? offs : d.len);

    auto i(stack.end() - offs), j(i + len);
    copy(i, j, back_inserter(stack));
  }

  void CpType::read(Cx &cx,
                    Pos &pos,
                    istream &in,
                    ReadState &state,
                    Env &env,
                    Stack &stack,
                    Ops &out) const {
    optional<Val> offs, len;
    bool done(false);
    Pos p(pos), vp(p);

    if (!(offs = read_val(pos, in, state, env, stack))) {
      throw ESys(vp, "Missing ;");
    }

    if (offs->is_eop()) {
      offs.emplace(pos, cx.int_type, Int(1));
      len.emplace(pos, cx.int_type, Int(1));
      done = true;
    } else {
      if (offs->type == &cx.bool_type && offs->as_bool) {
        offs.emplace(pos, cx.int_type, Int(-1));
      } else if (offs->type != &cx.int_type) {
        throw ESys(vp, "Expected Int, was: ", offs->type->id);
      }
      
      vp = pos;
      
      if (!(len = read_val(pos, in, state, env, stack))) {
        throw ESys(vp, "Missing ;");
      }

      if (len->is_eop()) {
        len.emplace(pos, cx.int_type, Int(1));
        done = true;
      } else if (len->type == &cx.bool_type && len->as_bool) {
        len.emplace(pos, cx.int_type, Int(-1));        
      } else if (len->type != &cx.int_type) {
        throw ESys(vp, "Expected Int, was: ", len->type->id);
      }
    }

    if (!done) { read_eop(pos, in, env, stack); }
    out.emplace_back(cx, p, *this, offs->as_int, len->as_int);
  }
}
