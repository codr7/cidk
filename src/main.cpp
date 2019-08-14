#include <iostream>
#include <cassert>

#include "cidk/cx.hpp"
#include "cidk/defer.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/repl.hpp"

using namespace std;
using namespace cidk;

enum struct Mode {load, repl};

int main(int argc, char *argv[]) {  
  Cx cx;
  Env env(cx, cx.env);
  const Pos &p(Pos::_);
  Mode m(Mode::repl);
  
  while (--argc && ++argv) {
    string a(*argv);
    
    if (a == "-debug") {
      cx.debug = true;
    } else {
      Ops ops;  
      Opts opts;
      cx.load(p, a, read_ops, env, ops, opts);
      cx.eval(ops, env, opts, cx.regp);
      m = Mode::load;
    }
  }

  if (m == Mode::repl) { repl(cx, cin, cout); }
  if (cx.debug) { cx.deinit(); }
  return 0;
}
