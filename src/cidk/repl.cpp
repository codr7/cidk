#include <sstream>

#include "cidk/cidk.hpp"
#include "cidk/cx.hpp"
#include "cidk/env.hpp"
#include "cidk/ext_id.hpp"
#include "cidk/pos.hpp"
#include "cidk/read.hpp"
#include "cidk/repl.hpp"

namespace cidk {
  void repl(Cx &cx, const string &header, Read read, istream &in, ostream &out) {
    out <<
      header << endl << endl <<
      "Press Return on empty row to evaluate." << endl <<
      "Empty input clears stack and Ctrl+D exits." << endl << endl <<
      "  ";

    Env &env(cx.env);
    Pos p("n/a");
    stringstream buf;
    string line;
    Ops ops;
    Opts opts;

    while (getline(in, line)) {
      if (line.empty()) {
        ops.clear();

        if (buf.tellp()) {
          try {
            read(cx, p, buf, ops);
            cx.compile(ops, opts, env);
            cx.eval(ops, env, cx.regp);
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
