#include <iostream>
#include <cassert>

#include "cidk/cx.hpp"
#include "cidk/ops/push.hpp"
#include "cidk/repl.hpp"
#include "cidk/val.hpp"

using namespace std;

void test(cidk::Cx &cx) {
  cidk::Stack s;
  
  auto p(cidk::Pos::_);
  
  cidk::Val v(p, cx.int_type, cidk::Int(42));
  assert(v.as_int == 42);
  assert(v.eq(p, v));

  v.splat(p, cx.env, s);
  assert(s.size() == 42);
  assert(s.back().as_int == 41);
}

enum struct Mode {load, repl};

int main(int argc, char *argv[]) {
  auto p(cidk::Pos::_);
  Mode m(Mode::repl);
  
  cidk::Cx cx;
  cidk::Env &env(*cx.env_pool.get(cx.env));
  cidk::Stack stack;

  test(cx);
  
  while (--argc && ++argv) {
    string a(*argv);

    if (a == "-debug") { cx.debug = true; }
    else {
      cidk::Ops ops;
      cx.load(p, a, ops);    
      cx.eval(ops, env, stack);
      m = Mode::load;
    }
  }

  if (m == Mode::repl) { cidk::repl(cx, cin, cout); }
  
  if (cx.debug) { cx.deinit(); }
  return 0;
}
