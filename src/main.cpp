#include <iostream>
#include <cassert>

#include "cidk/cx.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/repl.hpp"

using namespace std;

enum struct Mode {load, repl};

int main(int argc, char *argv[]) {
  const cidk::Pos &p(cidk::Pos::_);
  Mode m(Mode::repl);
  
  cidk::Cx cx;
  cidk::Env &env(*cx.env_pool.get(cx, cx.env));
  cidk::Stack stack;
  
  while (--argc && ++argv) {
    string a(*argv);
    
    if (a == "-debug") { cx.debug = true; }
    else {
      cidk::Ops ops;  
      cidk::Opts opts;
      cx.load(p, a, env, stack, ops, opts);    

      cidk::Regs regs;
      cx.eval(ops, env, regs, stack);
      m = Mode::load;
    }
  }

  if (m == Mode::repl) { cidk::repl(cx, cin, cout); }  
  if (cx.debug) { cx.deinit(); }
  return 0;
}
