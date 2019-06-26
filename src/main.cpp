#include <cassert>

#include "cidk/cx.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/val.hpp"

int main() {
  cidk::Cx cx;
  auto &s(cx.stack);
  
  auto p(cidk::Pos::_);
  
  cidk::Val v(p, cx.Int, cidk::Int(42));
  assert(v.as<cidk::Int>() == 42);
  assert(v.eq(p, v));

  v.splat(p);
  assert(s.size() == 42);
  assert(s.back().as_int == 41);

  return 0;
}
