#include <cctype>
#include <iostream>
#include <sstream>

#include "cidk/cx.hpp"
#include "cidk/e.hpp"
#include "cidk/read.hpp"
#include "cidk/str.hpp"
#include "cidk/types/expr.hpp"
#include "cidk/types/list.hpp"
#include "cidk/types/macro.hpp"
#include "cidk/types/pop.hpp"
#include "cidk/types/str.hpp"
#include "cidk/types/sym.hpp"

namespace cidk {
  void read_ops(Pos &pos,
                istream &in,
                ReadState &state,
                Env &env,
                Stack &stack,
                Ops &out) {
    while (read_op(pos, in, state, env, stack, out));
  }

  bool read_op(Pos &pos,
               istream &in,
               ReadState &state,
               Env &env,
               Stack &stack,
               Ops &out) {
    Cx &cx(env.cx);
    Pos p(pos);
    auto idv(read_val(pos, in, state, env, stack));

    if (!idv) {
      if (in.eof()) { return false; }
      throw ESys(p, "Missing op code");
    }

    if (idv->type != &cx.sym_type) {
      throw ESys(p, "Invalid op code: ", idv->type->id);
    }
    
    auto id(idv->as_sym->name);
    auto found(cx.op_types.find(id));
    if (id == "") { throw exception(); }
    if (found == cx.op_types.end()) { throw ESys(pos, "Unknown op: ", id); } 
    OpType &ot(*found->second);
    ot.read(cx, pos, in, state, env, stack, out);
    return true;
  }

  void read_eop(Pos &pos, istream &in, Env &env, Stack &stack) {
    Pos p = pos;
    ReadState s;
    auto v(read_val(pos, in, s, env, stack));
    if (!v || !v->is_eop()) { throw ESys(p, "Missing ;"); }
  }

  optional<Val> read_val(Pos &pos,
                         istream &in,
                         ReadState &state,
                         Env &env,
                         Stack &stack) {
    Cx &cx(env.cx);
    skip_ws(pos, in);
    
    if (char c(0); in.get(c)) {
      switch (c) {
      case '{':
        pos.col++;
        return read_expr(pos, in, state, env, stack);
      case '(':
        pos.col++;
        return read_list(pos, in, state, env, stack);
      case '"':
        pos.col++;
        return read_str(cx, pos, in);
      case '_':
        pos.col++;
        return cx._;
      case ';':
        pos.col++;
        return cx.eop;
      default:
        in.unget();
        if (isdigit(c)) { return read_num(cx, pos, in); }
        if (isgraph(c)) { return read_id(pos, in, state, env, stack); }
        throw ESys(pos, "Invalid input: ", c);
      };
    }
    
    return {};
  }
  
  Val read_expr(Pos &pos, istream &in, ReadState &state, Env &env, Stack &stack) {
    Cx &cx(env.cx);
    Pos p(pos);
    Expr *out(cx.expr_type.pool.get(cx));
    char c(0);
    
    for (;;) {
      skip_ws(pos, in);
      if (!in.get(c)) { throw ESys(pos, "Open expr"); }

      if (c == '}') {
        pos.col++;
        break;
      }
      
      in.unget();

      if (!read_op(pos, in, state, env, stack, out->body)) {
        throw ESys(pos, "Open expr");
      }
    }
    
    return Val(p, cx.expr_type, out);
  }

  Val read_id(Pos &pos, istream &in, ReadState &state, Env &env, Stack &stack) {
    Cx &cx(env.cx);
    Pos p(pos);
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
    auto id(cx.intern(out.str()));

    auto i(env.try_get(id));

    if (i) {
      Val &v(i->val);
      if (v.type->is_const) { return v.clone(p, v); }

      if (v.type == &cx.macro_type) {
        auto &et(cx.expr_type);
        Expr *e(et.pool.get(cx));
        v.as_macro->call(pos, in, env, stack, e->body);
        return Val(p, cx.expr_type, e);
      }

      return v;
    }
    
    return Val(p, cx.sym_type, id);
  }
  
  Val read_list(Pos &pos, istream &in, ReadState &state, Env &env, Stack &stack) {    
    Cx &cx(env.cx);
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
      auto v(read_val(pos, in, state, env, stack));
      if (!v) { throw ESys(pos, "Open list"); }
      out->items.push_back(*v);
    }
    
    return Val(p, cx.list_type, out);
  }
  
  Val read_num(Cx &cx, Pos &pos, istream &in) {
    Pos p(pos);
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
    
    return Val(p, cx.str_type, cx.str_type.pool.get(cx, out.str()));
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
