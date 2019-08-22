#include <cctype>
#include <iostream>
#include <sstream>

#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/read.hpp"
#include "cidk/str.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/fix.hpp"
#include "cidk/types/list.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/types/str.hpp"
#include "cidk/types/sym.hpp"

namespace cidk {
  void read_ops(Cx &cx, Pos &pos, istream &in, Ops &out) {
    while (read_op(cx, pos, in, out));
  }

  bool read_op(Cx &cx, Pos &pos, istream &in, Ops &out) {
    Pos p(pos);
    auto idv(read_val(cx, pos, in));

    if (!idv) {
      if (in.eof()) { return false; }
      throw ESys(p, "Missing op code");
    }

    if (idv->type != &cx.sym_type) {
      throw ESys(p, "Invalid op code: ", idv->type->id);
    }
    
    auto id(idv->as_sym->name);
    auto &ts(cx.op_types);
    auto found(ts.find(id));
    if (id == "") { throw exception(); }
    if (found == ts.end()) { throw ESys(p, "Unknown op: ", id); } 
    found->second->read(cx, pos, in, out);
    return true;
  }

  void read_eop(Pos &pos, istream &in) {
    Pos p = pos;
    skip_ws(pos, in);
    char c(0);
    
    if (!in.get(c) || c != ';') { throw ESys(p, "Missing ;"); }
    pos.col++;
  }

  optional<Val> read_val(Cx &cx, Pos &pos, istream &in) {
    skip_ws(pos, in);
    
    if (char c(0); in.get(c)) {
      switch (c) {
      case '{':
        pos.col++;
        return read_expr(cx, pos, in);
      case '(':
        pos.col++;
        return read_list(cx, pos, in);
      case '"':
        pos.col++;
        return read_str(cx, pos, in);
      case '\\':
        pos.col++;
        return read_ctrl_char(cx, pos, in);
      case '_':
        pos.col++;
        return cx._;
      case ';':
        pos.col++;
        return cx.eop;
      default:        
        if (c == '-') {
          char c1(in.get());
          in.unget();
          in.unget();
          if (isdigit(c1)) { c = c1; }
        } else {
          in.unget();
        }
        
        if (isdigit(c)) { return read_num(cx, pos, in); }
        if (isgraph(c)) { return read_id(cx, pos, in); }
        throw ESys(pos, "Invalid input: ", c);
      };
    }
    
    return {};
  }
  
  Val read_ctrl_char(Cx &cx, Pos &pos, istream &in) {    
    Pos p(pos);
    char c(0);
    if (!in.get(c)) { throw ESys(pos, "Invalid char literal"); }
    wchar_t wc(0);
    
    switch (c) {
    case '"':
      wc = L'"';
      break;
    default:
      throw ESys(pos, "Invalid char literal: ", c);
    }

    return Val(cx.char_type, wc);
  }

  Val read_expr(Cx &cx, Pos &pos, istream &in) {
    Pos p(pos);
    Expr *out(cx.expr_type.pool.get(cx));
    char c(0);
    
    for (;;) {
      skip_ws(pos, in);
      if (!in.get(c)) { throw ESys(pos, "Open expression"); }

      if (c == '}') {
        pos.col++;
        break;
      }
      
      in.unget();

      if (!read_op(cx, pos, in, out->ops)) {
        throw ESys(pos, "Open expression");
      }
    }

    return Val(cx.expr_type, out);
  }

  Val read_id(Cx &cx, Pos &pos, istream &in) {
    Pos p(pos);
    stringstream out;
    int arg_depth(0);
    char c(0);
    
    for (;;) {  
      if (!in.get(c) ||
          (!arg_depth && (!isgraph(c) ||
                          c == '(' || c == ')' ||
                          c == '{' || c == '}' ||
                          c == ']' || c == ';'))) { break; }
      
      out << c;

      switch (c) {
      case '[':
        arg_depth++;
        break;
      case ']':
        arg_depth--;
        break;
      default:
        break;
      }

      pos.col++;
    }

    if (!in.eof()) { in.unget(); }
    return Val(cx.sym_type, cx.intern(p, out.str()));
  }
  
  Val read_list(Cx &cx, Pos &pos, istream &in) {    
    Pos p(pos);
    List *out(cx.list_type.pool.get(cx));
    char c(0);
    
    for (;;) {
      if (!in.get(c)) { throw ESys(pos, "Open list"); }

      if (c == ')') {
        pos.col++;
        break;
      }
      
      in.unget();
      auto v(read_val(cx, pos, in));
      if (!v) { throw ESys(pos, "Open list"); }
      out->items.push_back(*v);
    }
    
    return Val(cx.list_type, out);
  }
  
  Val read_int(Cx &cx, Pos &pos, istream &in) {
    Pos p(pos);
    stringstream out;
    char c(0);
    Int v(0);
    bool is_neg(false);
    
    while (in.get(c) && (isdigit(c) || (c == '-' && !v))) {
      if (c == '-') {
        is_neg = true;
      } else {
        v = v * 10 + c - '0';
      }
        
      pos.col++;
    }

    if (!in.eof()) { in.unget();}
    return Val(cx.int_type, is_neg ? -v : v);
  }

  pair<uint64_t, uint8_t> read_frac(Cx &cx, Pos &pos, istream &in) {
    char c(0);    
    uint64_t v(0);
    uint8_t s(0);
    
    while (in.get(c) && isdigit(c)) {
      v = v * 10 + c - '0';
      s++;
      pos.col++;
    }

    if (!in.eof()) { in.unget();}
    return make_pair(v, s);
  }
  
  Val read_num(Cx &cx, Pos &pos, istream &in) {
    Val n(read_int(cx, pos, in));
    char c(0);
    
    if (!in.get(c) || c != '.') {
      in.unget();
      return n;
    }
    
    auto f(read_frac(cx, pos, in));
    
    return Val(cx.fix_type,
               fix::make(n.as_int * fix::pow(f.second) + f.first, f.second));
  }

  Val read_str(Cx &cx, Pos &pos, istream &in) {    
    Pos p(pos);
    stringstream out;
    char c(0), pc(0);
    
    for (;;) {
      pc = c;
      if (!in.get(c)) { throw ESys(pos, "Open str"); }
      pos.col++;

      if (pc != '\\') {
        if (c == '"') { break; }
        if (c == '\\') { continue; }
      }

      out << c;
    }
    
    return Val(cx.str_type, cx.str_type.pool.get(cx, out.str()));
  }

  void skip_ws(Pos &pos, istream &in) {
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
