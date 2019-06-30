#include <cctype>
#include <iostream>
#include <sstream>

#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/reader.hpp"
#include "cidk/str.hpp"
#include "cidk/types/list.hpp"
#include "cidk/types/sym.hpp"

namespace cidk {
  Reader::Reader(Cx &cx, const Pos &pos, istream &in):
    cx(cx), pos(pos), in(in), env(cx.env) {
  }

  void Reader::read_ops(Ops &out) {
    while (read_op(out));
  }

  bool Reader::read_op(Ops &out) {
    Pos p(pos);
    auto idv(read_val());

    if (!idv) {
      if (in.eof()) { return false; }
      throw ReadE(p, "Missing op code");
    }

    if (idv->type != &cx.sym_type) {
      throw WrongType(p, "Invalid op code: ", idv->type);
    }
    
    auto id(idv->as_sym->name);
    auto found(cx.op_types.find(id));
    if (found == cx.op_types.end()) { throw UnknownOp(pos, id); } 
    OpType &ot(*found->second);
    ot.read(cx, p, *this, out);
    return true;
  }

  void Reader::read_eop() {
    Pos p = pos;
    auto v(read_val());
    if (!v) { throw ReadE(p, "Missing ;"); }
    if (!v->is_eop()) { throw ReadE(p, "Expected ;"); }
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
      goto next;
    case ' ':
      pos.col++;
      goto next;
    case '\t':
      pos.col += 2;
      goto next;
    case '(':
      pos.col++;
      return read_list();
    case ';':
      pos.col++;
      return cx.eop;
    default:
      in.unget();
      if (isdigit(c)) { return read_num(); }
      if (isgraph(c)) { return read_id(); }
      throw ReadE(str("Invalid input: ", c));
    };
    
    return {};
  }

  Val Reader::read_id() {
    const Pos p(pos);
    stringstream out;
    
    for (;;) {
      char c(0);
      
      if (!in.get(c) ||
          c == '(' || c == ')' || c == ';' || 
          !isgraph(c)) { break; }
      
      out << c;
      pos.col++;
    }

    if (!in.eof()) { in.unget();}
    return Val(p, cx.sym_type, cx.intern(out.str()));
  }
  
  Val Reader::read_list() {
    Pos p(pos);
    List *out(cx.list_type.pool.get(cx));
    
    for (;;) {
      char c(0);
      if (!in.get(c)) { throw ReadE(pos, "Open list"); }
      if (c == ')') { break; }
      in.unget();
      auto v(read_val());
      if (!v) { throw ReadE(pos, "Open list"); }
      out->items.push_back(*v);
    }
    
    return Val(p, cx.list_type, out);
  }
  
  Val Reader::read_num() {
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
}
