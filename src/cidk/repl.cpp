#include <sstream>

#include "cidk/cidk.hpp"
#include "cidk/cx.hpp"
#include "cidk/env.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/pos.hpp"
#include "cidk/read.hpp"
#include "cidk/repl.hpp"

namespace cidk {
  void repl(Cx &cx, istream &in, ostream &out) {
    out <<
      "cidk v" << VERSION[0] << '.' << VERSION[1] << endl << endl <<
      "Press Return on empty row to evaluate." << endl <<
      "Empty input clears stack and Ctrl+D exits." << endl << endl <<
      "  ";

    Env env(cx, cx.env);
    Pos p(Pos::_);
    stringstream buf;
    string line;

    while (getline(in, line)) {
      if (line.empty()) {
        if (buf.tellp()) {
          try {
            Ops ops;
            Opts opts;
            read_ops(cx, p, buf, ops);
            cx.compile(ops, opts, env);

            for (auto &eid: opts.ext_ids) {
              throw ESys(eid.pos, "Unknown id: ", eid.id);
            }

            cx.eval(ops, env, opts);
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
