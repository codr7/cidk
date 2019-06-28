#include <iostream>
#include <cassert>

#include "cidk/cx.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/val.hpp"

using namespace std;

void test(cidk::Cx &cx) {
  auto &s(cx.stack);
  auto p(cidk::Pos::_);
  
  cidk::Val v(p, cx.Int, cidk::Int(42));
  assert(v.as<cidk::Int>() == 42);
  assert(v.eq(p, v));

  v.splat(p);
  assert(s.size() == 42);
  assert(s.back().as_int == 41);
}

int main(int argc, char *argv[]) {
  auto p(cidk::Pos::_);
  cidk::Cx cx;
  test(cx);

  while (--argc && ++argv) {
    cidk::Ops ops;
    cx.load(p, *argv, ops);
    cx.eval(p, ops);
  }
  
  return 0;
}
