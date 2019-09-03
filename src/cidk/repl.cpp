#include <sstream>

#include "cidk/cidk.hpp"
#include "cidk/cx.hpp"
#include "cidk/env.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/e_user.hpp"
#include "cidk/pos.hpp"
#include "cidk/read.hpp"
#include "cidk/repl.hpp"

namespace cidk {
  void repl(Cx &cx, istream &in, ostream &out) {
    out <<
      "cidk v" << VERSION << endl << endl <<
      "Press Return on empty row to evaluate." << endl <<
      "Empty input clears stack and Ctrl+D exits." << endl << endl <<
      "  ";

    Pos p;
    stringstream buf;
    string line;

    while (getline(in, line)) {
      if (line.empty()) {
        if (buf.tellp()) {
          try {
            Ops ops;
            Opts opts;
            read_ops(cx, p, buf, ops);
            cx.compile(ops, opts, cx.env);

            for (auto &eid: opts.ext_ids) {
              throw ESys(eid.pos, "Unknown id: ", eid.id);
            }
            
            Reg *eval_regs(cx.alloc_regs(opts.regs.size()));
            cx.eval(ops, cx.env, eval_regs);
            cx.regp = eval_regs;

            for (Val *v(eval_regs); v < eval_regs + opts.regs.size(); v++) {
              if (v->type) { cx.env.set(cx, p, v->id, *v, true); }
            }

            if (cx.e) {
              out << cx.e->what() << endl;
              cx.e = nullptr;
            }
          } catch (const exception &e) {
            out << e.what() << endl;
          }
        } else {
          cx.stackp = &cx.stack[0];
        }
        
        cx.dump_stack(out);
        out << endl;
        stringstream().swap(buf);
      } else {
        buf << line << endl;
      }

      out << "  ";
    }
  }
}
