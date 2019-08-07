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

  void CpType::eval(Cx &cx, Op &op, Env &env, Reg *regs, Stack &stack) const {
    const auto &d(op.as<CpData>());
    auto ss(stack.size());
      
    auto
      offs((d.offs == -1) ? ss - 1 : d.offs),
      len((d.len == -1) ? offs + 1 : d.len);

    if (len == 1) {
      stack.push_back(stack[ss - offs - 1]);
    } else {
      auto i(stack.end() - offs - 1), j(i + len);
      copy(i, j, back_inserter(stack));
    }
  }

  void CpType::read(Cx &cx, Pos &pos, istream &in, Ops &out) const {
    Int offs(0), len(1);
    bool done(false);
    Pos p(pos), vp(p);
    
    auto v(read_val(cx, pos, in));
    if (!v) { throw ESys(vp, "Missing ;"); }

    if (v->is_eop()) {
      done = true;
    } else {
      if (v->type == &cx.bool_type && v->as_bool) {
        offs = -1;
      } else if (v->type == &cx.int_type) {
        offs = v->as_int;
      } else {
        throw ESys(p, "Invalid copy offset, expected Int: ", v->type->id);
      }

      if (!(v = read_val(cx, pos, in))) { throw ESys(vp, "Missing ;"); }

      if (v->is_eop()) {
        done = true;
      } else {
        if (v->type == &cx.bool_type && v->as_bool) {
          len = -1;
        } else if (v->type == &cx.int_type) {
          len = v->as_int;
        } else {
          throw ESys(p, "Invalid copy length, expected Int: ", v->type->id);
        }
      }
    }

    if (!done) { read_eop(pos, in); }
    out.emplace_back(cx, p, *this, offs, len);
  }
}
