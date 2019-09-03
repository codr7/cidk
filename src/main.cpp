#include <iostream>
#include <cassert>

#include "cidk/cx.hpp"
#include "cidk/defer.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/e_user.hpp"
#include "cidk/libs/abc.hpp"
#include "cidk/libs/math.hpp"
#include "cidk/repl.hpp"

using namespace std;
using namespace cidk;

enum struct Mode {load, repl};

int main(int argc, char *argv[]) {  
  Cx cx;
  Mode m(Mode::repl);

  Lib &abc(libs::init_abc(cx, Pos::_));
  cx.env.use(cx, Pos::_, abc.env);

  Lib &math(libs::init_math(cx, Pos::_));
  cx.env.use(cx, Pos::_, math.env);

  while (--argc && ++argv) {
    string a(*argv);
    
    if (a == "-debug") {
      cx.debug = true;
    } else {
      Ops ops;  
      Opts opts;
      cx.load(Pos::_, a, read_ops, cx.env, ops, opts);
      for (auto &eid: opts.ext_ids) { throw ESys(eid.pos, "Unknown id: ", eid.id); }
      Reg *eval_regs(cx.alloc_regs(opts.regs.size()));
      cx.eval(ops, cx.env, eval_regs);
      cx.regp = eval_regs;

      for (Val *v(eval_regs); v < eval_regs + opts.regs.size(); v++) {
        if (v->type) { cx.env.set(cx, Pos::_, v->id, *v, true); }
      }

      if (cx.e) { throw *cx.e; }
      m = Mode::load;
    }
  }

  if (m == Mode::repl) { repl(cx, cin, cout); }
  if (cx.debug) { cx.deinit(); }
  return 0;
}
