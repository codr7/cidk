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
  const Pos &p(Pos::_);
  Mode m(Mode::repl);
  
  while (--argc && ++argv) {
    string a(*argv);
    
    if (a == "-debug") {
      cx.debug = true;
    } else {
      Ops ops;  
      Opts opts;
      cx.load(p, a, read_ops, cx.env, ops, opts);
      for (auto &eid: opts.ext_ids) { throw ESys(eid.pos, "Unknown id: ", eid.id); }
      cx.eval(ops, cx.env, cx.regp);

      for (Val *v(cx.regp); v < cx.regp + opts.regs.size(); v++) {
        if (v->type) { cx.env.set(cx, p, v->id, *v, true); }
      }

      m = Mode::load;
    }
  }

  if (m == Mode::repl) { repl(cx, cin, cout); }
  if (cx.debug) { cx.deinit(); }
  return 0;
}
