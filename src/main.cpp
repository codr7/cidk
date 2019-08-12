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
  const Pos &p(Pos::_);
  Mode m(Mode::repl);
  
  Cx cx;
  Env &env(cx.env);
  
  while (--argc && ++argv) {
    string a(*argv);
    
    if (a == "-debug") { cx.debug = true; }
    else {
      Ops ops;  
      Opts opts;
      cx.load(p, a, read_ops, env, ops, opts);

      Reg *regs(cx.regp);
      defer({cx.regp = regs;});
      cx.regp += opts.regs.size();
      for (auto &src: opts.ext_ids) { regs[src.dst_reg] = src.val; }
      cx.eval(ops, env, regs);
      
      m = Mode::load;
    }
  }

  if (m == Mode::repl) {
    repl(cx, str("cidk v", VERSION[0], '.', VERSION[1]), read_ops, cin, cout);
  }
  
  if (cx.debug) { cx.deinit(); }
  return 0;
}
