#include <cctype>
#include <iostream>
#include <sstream>

#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/reader.hpp"
#include "cidk/str.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/list.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/types/sym.hpp"

namespace cidk {
  Reader::Reader(Cx &cx, const Pos &pos, istream &in):
    cx(cx), pos(pos), in(in) { }

  void Reader::read_ops(Env &env, Ops &out) { while (read_op(env, out)); }

  bool Reader::read_op(Env &env, Ops &out) {
    Pos p(pos);
    auto idv(read_val(env));

    if (!idv) {
      if (in.eof()) { return false; }
      throw ERead(p, "Missing op code");
    }

    if (idv->type != &cx.sym_type) {
      throw EWrongType(p, "Invalid op code: ", idv->type);
    }
    
    auto id(idv->as_sym->name);
    auto found(cx.op_types.find(id));
    if (id == "") { throw exception(); }
    if (found == cx.op_types.end()) { throw EUnknownOp(pos, id); } 
    OpType &ot(*found->second);
    ot.read(cx, p, *this, env, out);
    return true;
  }

  void Reader::read_eop(Env &env) {
    Pos p = pos;
    auto v(read_val(env));
    if (!v) { throw ERead(p, "Missing ;"); }
    if (!v->is_eop()) { throw ERead(p, "Expected ;"); }
  }

  optional<Val> Reader::read_val(Env &env) {
    skip_ws();
    
    if (char c(0); in.get(c)) {
      switch (c) {
      case '$':
        return read_pop(env);
      case '(':
        pos.col++;
        return read_list(env);
      case '{':
        pos.col++;
        return read_expr(env);
      case ';':
        pos.col++;
        return cx.eop;
      default:
        in.unget();
        if (isdigit(c)) { return read_num(env); }
        if (isgraph(c)) { return read_id(env); }
        throw ERead(str("Invalid input: ", c));
      };
    }
    
    return {};
  }
  
  Val Reader::read_expr(Env &env) {
    Pos p(pos);
    Expr *out(cx.expr_type.pool.get(cx));
    char c(0);
    
    for (;;) {
      skip_ws();
      if (!in.get(c)) { throw ERead(pos, "Open expr"); }

      if (c == '}') {
        pos.col++;
        break;
      }
      
      in.unget();
      if (!read_op(env, out->body)) { throw ERead(pos, "Open expr"); }
    }
    
    return Val(p, cx.expr_type, out);
  }

  Val Reader::read_id(Env &env) {
    const Pos p(pos);
    stringstream out;
    char c(0);
    
    for (;;) {  
      if (!in.get(c) ||
          c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || 
          !isgraph(c)) { break; }
      
      out << c;
      pos.col++;
    }

    if (!in.eof()) { in.unget();}
    return Val(p, cx.sym_type, cx.intern(out.str()));
  }
  
  Val Reader::read_list(Env &env) {
    Pos p(pos);
    List *out(cx.list_type.pool.get(cx));
    char c(0);
    
    for (;;) {
      if (!in.get(c)) { throw ERead(pos, "Open list"); }

      if (c == ')') {
        pos.col++;
        break;
      }
      
      in.unget();
      auto v(read_val(env));
      if (!v) { throw ERead(pos, "Open list"); }
      out->items.push_back(*v);
    }
    
    return Val(p, cx.list_type, out);
  }
  
  Val Reader::read_num(Env &env) {
    const Pos p(pos);
    stringstream out;
    char c(0);
    
    for (;;) {
      if (!in.get(c) || !isdigit(c)) { break; }
      out << c;
      pos.col++;
    }

    if (!in.eof()) { in.unget();}
    Int n(strtoll(out.str().c_str(), NULL, 10));
    return Val(p, cx.int_type, n);
  }

  Val Reader::read_pop(Env &env) {
    Val v(cx.pop_type);
    pos.col++;
    return v;
  }

  void Reader::skip_ws() {
    char c(0);

    while (in.get(c) && isspace(c)) {
      switch (c) {
      case ' ':
        pos.col++;
        break;
      case '\t':
        pos.col += 2;
        break;
      case '\n':
        pos.row++;
        pos.col = 0;
      };
    }
    
    if (!isspace(c)) { in.unget(); }
  }
}
