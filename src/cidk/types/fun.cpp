#include "cidk/call.hpp"
#include "cidk/types/fun.hpp"
#include "cidk/val.hpp"

namespace cidk {
  FunType::FunType(Cx &cx, const Pos &pos, string id):
    ValTType<Fun *>(cx, pos, id) { }

  void FunType::call(const Pos &pos, const Val &val) const {
    Call(cx, pos, *val.as_fun).eval();
  }

  void FunType::dup(const Pos &pos, Val &dst, const Val &src) const {
    dst.as_fun = src.as_fun;
  }

  bool FunType::is(const Pos &pos, const Val &x, const Val &y) const {
    return x.as_fun == y.as_fun;
  }

  void FunType::set(const Pos &pos, Val &dst, Fun *val) const {
    dst.as_fun = val;
  }

  void FunType::sweep(const Pos &pos, Val &val) {
    pool.put(val.as_fun);
  }
}
