#ifndef CIDK_READ_HPP
#define CIDK_READ_HPP

#include "cidk/ops.hpp"

namespace cidk {
  struct Env;
  struct Pos;
  struct Val;
  
  void read_ops(Pos &pos,
                istream &in,
                ReadState &state,
                Env &env,
                Stack &stack,
                Ops &out);
  
  bool read_op(Pos &pos,
               istream &in,
               ReadState &state,
               Env &env,
               Stack &stack,
               Ops &out);
  
  void read_eop(Pos &pos, istream &in, Env &env, Stack &stack);
  
  optional<Val> read_val(Pos &pos,
                         istream &in,
                         ReadState &state,
                         Env &env,
                         Stack &stack);
  
  Val read_ctrl(Cx &cx, Pos &pos, istream &in);
  Val read_expr(Pos &pos, istream &in, ReadState &state, Env &env, Stack &stack);
  Val read_id(Pos &pos, istream &in, ReadState &state, Env &env, Stack &stack);
  Val read_list(Pos &pos, istream &in, ReadState &state, Env &env, Stack &stack);
  Val read_num(Cx &cx, Pos &pos, istream &in);
  Val read_str(Cx &cx, Pos &pos, istream &in);

  void skip_ws(Pos &pos, istream &in);
}

#endif
