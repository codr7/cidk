#include <cctype>
#include <iostream>
#include <sstream>

#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/reader.hpp"
#include "cidk/str.hpp"

namespace cidk {
  Reader::Reader(Cx &cx, const Pos &pos, istream &in): cx(cx), pos(pos), in(in), indent(0) { }
  
  void Reader::read_op(Ops &out) {
    indent = read_indent();
    
    const Pos op(pos);
    string opc;
    in >> opc;
    pos.col += opc.size();
    
    cout << "opc: " << opc << endl;
    auto found(cx.op_types.find(opc));
    if (found == cx.op_types.end()) { throw UnknownOp(pos, opc); } 
    OpType &ot(*found->second);
    ot.read(cx, op, *this, out);
  }
  
  void Reader::read_ops(Ops &out) {
    while (!in.eof()) { read_op(out); }
  }

  optional<Val> Reader::read_val() {
  next:
    char c = in.get();
    
    switch (c) {
    case EOF:
      break;
    case '\n':
      pos.row++;
      pos.col = 0;
      indent = read_indent();
      goto next;
    case ' ':
      pos.col++;
      goto next;
    case '(':
      return read_list();
    default:
      if (isdigit(c)) { return read_num(); }
      if (isgraph(c)) { return read_id(); }
      throw ReadE(str("Invalid input: ", c));
    };
    
    return {};
  }

  optional<Val> Reader::read_id() {
    return {};
  }
  
  optional<Val> Reader::read_list() {
    return {};
  }
  
  optional<Val> Reader::read_num() {
    auto p(pos);
    stringstream out;
    char c(0);
    
    for (;;) {
      if (!in.get(c) || !isdigit(c)) { break; }
      out << c;
    }

    if (!in.eof()) { in.unget();}
    Int n(strtoll(out.str().c_str(), NULL, 10));
    return Val(p, cx.Int, n);
  }

  int Reader::read_indent() {
    int n(0);
    char c(0);
    
    for (;;) {
      if (!in.get(c)) { break; }
      if (c == ' ') { n++; }
      else if (c == '\t') { n += 2; }
      else { break; }
    }

    if (!in.eof()) { in.unget(); }
    return n;
  }
}
