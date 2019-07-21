#include <sstream>

#include "cidk/cidk.hpp"
#include "cidk/cx.hpp"
#include "cidk/env.hpp"
#include "cidk/pos.hpp"
#include "cidk/read.hpp"
#include "cidk/repl.hpp"

namespace cidk {
  void repl(Cx &cx, istream &in, ostream &out) {
    out <<
      "cidk v" << VERSION[0] << '.' << VERSION[1] << endl << endl <<
      "Press Return on empty row to evaluate." << endl <<
      "Evaluating nothing clears stack and Ctrl+D exits." << endl << endl <<
      "  ";

    Env &env(cx.env);
    Stack stack;
    Pos p("repl");
    stringstream buf;
    string line;
    Ops ops;

    while (getline(in, line)) {
      if (line.empty()) {
        ops.clear();

        if (buf.tellp()) {
          read_ops(cx, p, buf, ops);
          cx.compile(ops, nullptr, env, stack);
          cx.eval(ops, env, stack);
        } else {
          stack.clear();
        }
        
        out << stack << endl;
        stringstream().swap(buf);
      } else {
        buf << line << endl;
      }

      out << "  ";
    }
  }
}
