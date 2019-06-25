#include <cassert>

#include "cidk/cx.hpp"
#include "cidk/ops/lit.hpp"
#include "cidk/val.hpp"

int main() {
  cidk::Cx cx;
  auto &s(cx.stack);
  
  auto p(cidk::Pos::MISSING);
  
  cidk::Val v(p, cx.Int, cidk::Int(42));
  assert(v.as<cidk::Int>() == 42);
  assert(v.eq(p, v));

  v.splat(p);
  assert(s.size() == 42);
  assert(s.back().as_int == 41);

  cidk::Stack in(s);
  s.clear();
  
  cidk::Ops out;
  cx.compile(p, in, out);
  assert(out.size() == 42);
  assert(out.back().type == &cidk::ops::Lit);

  assert(s.size() == 42);
  assert(s.back().as_int == 41);
  s.clear();
  
  cx.eval(p, out);
  assert(s.size() == 42);
  assert(s.back().as_int == 41);

  return 0;
}
