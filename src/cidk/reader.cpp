#include <cctype>
#include <iostream>
#include <sstream>

#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/reader.hpp"
#include "cidk/str.hpp"

namespace cidk {
  Reader::Reader(Cx &cx, const Pos &pos, istream &in):
    cx(cx), pos(pos), in(in), indent(0), env(cx.env) { }
  
  void Reader::read_op(Ops &out) {
    indent = read_indent();
    const Pos p(pos);
    auto idv(read_val());
    if (!idv) { throw ReadE(p, "Missing op code"); }

    if (idv->type != &cx.sym_type) {
      throw WrongType(p, "Invalid op code: ", idv->type);
    }
    
    auto id(idv->as_sym->name);
    cout << "opc: " << id << endl;
    auto found(cx.op_types.find(id));
    if (found == cx.op_types.end()) { throw UnknownOp(pos, id); } 
    OpType &ot(*found->second);
    ot.read(cx, p, *this, out);
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
      in.unget();
      if (isdigit(c)) { return read_num(); }
      if (isgraph(c)) { return read_id(); }
      throw ReadE(str("Invalid input: ", c));
    };
    
    return {};
  }

  optional<Val> Reader::read_id() {
    const Pos p(pos);
    stringstream out;
    
    for (;;) {
      char c(0);
      
      if (!in.get(c) ||
          c == '(' || c == ')' ||
          !isgraph(c)) { break; }
      
      out << c;
      pos.col++;
    }

    if (!in.eof()) { in.unget();}
    return Val(p, cx.sym_type, cx.intern(out.str()));
  }
  
  optional<Val> Reader::read_list() {
    return {};
  }
  
  optional<Val> Reader::read_num() {
    const Pos p(pos);
    stringstream out;
    
    for (;;) {
      char c(0);
      if (!in.get(c) || !isdigit(c)) { break; }
      out << c;
      pos.col++;
    }

    if (!in.eof()) { in.unget();}
    Int n(strtoll(out.str().c_str(), NULL, 10));
    return Val(p, cx.int_type, n);
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
    pos.col += n;
    return n;
  }
}
